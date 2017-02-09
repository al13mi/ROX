import argparse
import socket
import random
import capnp

import brain_capnp
import time

import numpy as np

from keras.models import Sequential
from keras.layers import Dense, LSTM, Dropout, Convolution1D, MaxPooling1D, GlobalMaxPooling1D, Embedding, Flatten
from keras.optimizers import SGD, Nadam, Adam, Adadelta
import math
import os
import scipy.stats
from scipy.stats.stats import pearsonr
import re
import random

from threading import Lock
mutex = Lock()
import pickle
import gc

total_packet_count = 0
NN_packet_count = 0
oracle_packet_count = 0
random_packet_count = 0

def calculateSortedness():
    global NN_packet_count
    global total_packet_count
    global oracle_packet_count
    global random_packet_count
    x =  os.popen("sudo ovs-ofctl dump-flows ovsbr0 -OOpenFlow14 | grep -v 'CONTROLLER' | awk '{print $4,$9}'").read()
    res = re.findall('\=([0-9]{1,6}).*\=([0-9]{1,6})', x)
    res = [(int(x),int(y)) for x,y in res]
    random.shuffle(res)
    A = res[:]
    B = res[:]
    B.sort(key=lambda x: x[1])
    A.sort(key=lambda x: x[0])
    C = res[len(res)/2:]
    D = B[len(res)/2:]
    E = A[len(res)/2:]
    # Calculate speedup
    old = sum([x for x,y in C])
    new = sum([x for x,y in D])
    oracle = sum([x for x,y in E])
    everything = sum([x for x,y in res])
    tau, p_value = scipy.stats.kendalltau(A,res)

    NN_packet_count += new
    oracle_packet_count += oracle
    total_packet_count += everything
    random_packet_count += old
    with open("total.txt", "a") as myfile:
        myfile.write("{0},{1},{2},{3}\n".format(NN_packet_count, oracle_packet_count, total_packet_count, random_packet_count))

    corr_x = [x for x,y in D]
    corr_y = [x for x,y in E]
    correlation, p_value = pearsonr(scipy.array(corr_x), scipy.array(corr_y))
    return tau, float(new) / old, float(oracle) / old, float(new)/everything, float(oracle)/everything, correlation, p_value
import sys

class PriorityImpl(brain_capnp.Brain.Priority.Server):
        def __init__(self, value):
            self.value = value

        def read(self, **kwargs):
            return self.value

def ConvertToOneHot(A):
    result = []
    for x in A:
        temp = [0]*5
        if x > 0.1:
            temp[4] = 1
        elif x > 0.01:
            temp[3] = 1
        elif x > 0.005:
            temp[2] = 1
        elif x > 0.001:
            temp[1] = 1
        else:
            temp[0] = 1
        result.append(temp)
    return result

from itertools import islice

class Dataset(object):
    def __init__(self, example, expected, count):
        big_end = len(expected)*75/100
        
        self.training_expected = expected[:big_end]
        self.training_example = example[:big_end]
        self.test_expected = expected[big_end:]
        self.test_example = example[big_end:]
        self.test_counts = count[big_end:]

def getModel():
    model = Sequential()

    model.add(Dense(50, input_dim=16, activation='relu', init='uniform'))
    model.add(Dropout(0.20))
    model.add(Dense(50, activation='relu', init='uniform'))
    model.add(Dropout(0.20))
    model.add(Dense(50, activation='relu', init='uniform'))
    model.add(Dropout(0.20))
    model.add(Dense(50, activation='relu', init='uniform'))
    model.add(Dropout(0.20))
    
    model.add(Dense(5, init='uniform', activation='sigmoid'))

    sgd = Adadelta()
    model.compile(loss='categorical_crossentropy', optimizer=sgd, metrics=['accuracy'])
    return model


class BrainImpl(brain_capnp.Brain.Server):
    def __init__(self):
        self.size = 1
        self.max = 1000.

        self.example = []
        self.expected = []
        self.count = []
        self.global_example = []
        self.global_expected = []

        gc.disable()
        temp = []
        print "Loading Dataset"
        for x in range(0,433):
            temp_expected,temp_example,temp_counts = pickle.load(open("/data/packets/save-{0}.p".format(x), "rb"))
            sys.stdout.write('.')
            sys.stdout.write("{0}".format(x))
            sys.stdout.flush()
            self.expected.extend(temp_expected)
            self.example.extend(temp_example)
            self.count.extend(temp_counts)
        self.generation = 0
        print len(self.example)
        print "Creating 25% Holdback for test data"
	
        self.model = getModel()
        Wsave = self.model.get_weights()
        N = 0
        for y in range(1,1001):
                self.model.set_weights(Wsave)
                slice_size = 1000*len(self.expected)/1000

                a = [x for x in islice(self.example, slice_size)]
                b = [x for x in islice(self.expected, slice_size)]
                c = [x for x in islice(self.count, slice_size)]
                data = Dataset(a, b, c)

		print len(data.training_example)
                print len(data.training_expected)
                self.model.fit(np.array(data.training_example), np.array(data.training_expected), batch_size=100000, nb_epoch=5000)
                self.store_results(c, a)

                good = 0
                bad = 0

                for example, expected in zip(data.test_example, data.test_expected):
                    result = self.model.predict(np.array([example]))
                    result_arg = np.argmax(result[0])
                    expect_arg = np.argmax(expected)

                    if result_arg == expect_arg:
                        good += 1
                    else:
                        bad += 1
                print "{0} {1}".format(good, bad)
                with open("output-perf.txt", "a") as myfile:
                    myfile.write("{0},{1},{2},{3},{4}\n".format(self.generation, y, good, bad, float(good)/(good + bad)))
                    self.generation += 1

        

        self.example = []
        self.expected = []
        self.exact_counts = []
        self.global_example = []
        self.global_expected = []
        gc.enable()

        self.generation = 0
        self.count = 0
        self.model.summary()
        #self.f = open("output.txt", "a")
        self.counter = 0
        self.save_counter = 0
        expire_buffer = []

    def store_results(self, counts, examples):
        #print examples

        testdata = []
        for x in examples:
            temp = np.array([x])
            temp = self.model.predict(temp)[0]
            temp = np.argmax(temp)
            temp += 1
            
            testdata.append(temp)
            
        #testdata = [(np.argmax(self.model.predict(np.array(x)))+1) for x in examples]
        result = zip(counts, examples, testdata)
        #print result
        total = sum([x for x,y,z in result])
        
        random.shuffle(result)
        result.sort(key=lambda x: x[0])
        oracle = sum([x for x,y,z in result[len(result)/2:]])

        random.shuffle(result)
        rand = sum([x for x,y,z in result[len(result)/2:]])
        result.sort(key=lambda x: x[2])
        nn = sum([x for x,y,z in result[len(result)/2:]])

        with open("output.txt", "a") as myfile:
            myfile.write("{0},{1},{2},{3}\n".format(self.generation, float(oracle)/total, float(nn)/total, float(nn)/float(rand)))
            self.generation += 1

        
def main():
        brain = BrainImpl()

if __name__ =='__main__':
        main()

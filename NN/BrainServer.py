import argparse
import socket
import random
import capnp

import brain_capnp
import time

import numpy as np

from keras.models import Sequential
from keras.layers import Dense, LSTM

class PriorityImpl(brain_capnp.Brain.Priority.Server):
        def __init__(self, value):
            self.value = value

        def read(self, **kwargs):
            return self.value

class BrainImpl(brain_capnp.Brain.Server):
    def __init__(self):
        self.size = 10
        self.max = 100000.

        self.model = Sequential()
        self.model.add(Dense(32, input_dim=16, init='uniform', activation='relu'))
        self.model.add(Dense(16, init='uniform', activation='relu'))
        #self.model.add(LSTM(32, input_dim=16, dropout_W=0.2, dropout_U=0.2))
        #self.model.add(LSTM(32, dropout_W=0.2, dropout_U=0.2))
        self.model.add(Dense(10, init='uniform', activation='sigmoid'))
        self.model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])

        self.example = []
        self.expected = []
        
    def predict(self, packet, _context, **kwargs):
        print "predict:"
        sample = [int(x.value) for x in packet.data]
        sample = np.array(sample)
        print sample
        input = np.array([sample])
        print input.shape
        result = self.model.predict(input)
        r = result[0]
        print r
        i = np.argmax(r)
        r = i/float(len(r))*10000
        print r
        return PriorityImpl(int(r))
        
    def learn(self, packet, priority, _context, **kwargs):
        print "learn:"
        l = [0]*self.size
        p = int(priority.value)
        print p
        p = p / self.max * self.size
        
        if p >= self.size-1:
            p = p - 1
        p = int(p)
        l[p] = 1
        p = np.array(l)
        print p
        sample = [int(x.value) for x in packet.data]
        sample = np.array(sample)
        #print sample
        #print p
        #print packet
        self.example.append(sample)
        self.expected.append(p)
        print len(self.example)
        if len(self.example) >= 100:
            self.model.fit(np.array(self.example), np.array(self.expected), nb_epoch=5, batch_size=25)
            scores = self.model.evaluate(np.array(self.example), np.array(self.expected))
            print("%s: %.2f%%" % (self.model.metrics_names[1], scores[1]*100))
            self.expected = []
            self.example = []
        
        
def main():
        address = "127.0.0.1:3333"
        brain = BrainImpl()
        server = capnp.TwoPartyServer(address, bootstrap=brain)
        server.run_forever()

if __name__ =='__main__':
        main()

import argparse
import socket
import random
import capnp

import brain_capnp
import time

from pylearn2.models import mlp
from pylearn2.training_algorithms import bgd, sgd
from pylearn2.termination_criteria import EpochCounter
from pylearn2.datasets.dense_design_matrix import DenseDesignMatrix
import numpy as np
import theano

class Samples(DenseDesignMatrix):
    def __init__(self, X, y):
        self.class_names = ['Input', 'Output']
        super(Samples, self).__init__(X=np.array(X),y=np.array(y))

class PriorityImpl(brain_capnp.Brain.Priority.Server):
        def __init__(self, value):
            self.value = value

        def read(self, **kwargs):
            return self.value

class BrainImpl(brain_capnp.Brain.Server):
    def __init__(self):
        hidden_layer = mlp.Sigmoid(layer_name='hidden', dim=64, irange=.1, init_bias=1.)
        self.size = 10
        self.max = 100000.
        output_layer = mlp.Softmax(self.size, 'output', irange=.1)
        #self.trainer = bgd.BGD(batch_size=32)
        self.trainer = sgd.SGD(learning_rate=.01, batch_size=32, termination_criterion=EpochCounter(400))
        layers = [hidden_layer, output_layer]
        self.ann = mlp.MLP(layers, nvis=16)
        self.example = []
        self.expected = []
        
    def predict(self, packet, _context, **kwargs):
        print "predict:"
        sample = [int(x.value) for x in packet.data]
        print sample
        input = np.array([sample])
        result = self.ann.fprop(theano.shared(input, name='inputs')).eval()
        r = result[0]
        print r
        #m = max(r)
        #r = [i for i,j in enumerate(r) if j==m]
        #print "One Hot: " + str(r)
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
        p = l
        print p
        sample = [int(x.value) for x in packet.data]
        #print sample
        #print p
        #print packet
        self.example.append(sample)
        self.expected.append(p)
        print len(self.example)
        if len(self.example) >= 32:
            ds = Samples([sample], [p])
            self.trainer.setup(self.ann, ds)
            epoch = 1
            #while True:
                #self.trainer.setup(self.ann, ds)
                #for x in range(0,10):
            self.trainer.train(dataset=ds)
            self.ann.monitor.report_epoch()
            self.ann.monitor()
            #if not self.trainer.continue_learning(self.ann):
            #    break
            #self.example = []
            #self.expected = []
        
        
def main():
        address = "127.0.0.1:3333"
        brain = BrainImpl()
        server = capnp.TwoPartyServer(address, bootstrap=brain)
        server.run_forever()

if __name__ =='__main__':
        main()

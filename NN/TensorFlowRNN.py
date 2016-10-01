def test_initialization():
    pass

def learn(crc, durationNSec, packetCount, byteCount, timestamp, header):
    pass

def predict(crc, header):
    return 15

print "foo"
import pybrain
print "bar"
#from pybrain.supervised.trainers import BackpropTrainer
#from pybrain.datasets import SupervisedDataSet
#from pybrain.tools.shortcuts import buildNetwork
#from pybrain.structure.modules import LSTMLayer, SoftmaxLayer

import collections
"""
rnn = None
samples = None
ds = None

def initialize_network():
    global rnn
    global samples
    global ds
    rnn = buildNetwork( 6, 100, 1, bias=True, hiddenclass=LSTMLayer, recurrent=False)

    samples = collections.deque(maxlen=100)

def test_initialization():
    print "Loading TensorFlow RNN"
    # initialize_network()

def learn(crc, durationNSec, packetCount, byteCount, timestamp, header):
    # add the sample to the queue
    global samples
    samples.append((header, (packetCount)))
    ds = SupervisedDataSet(6, 1)

    ds.addSample(header, packetCount)
    #rnn.activate(header)
    trainer = BackpropTrainer(rnn, ds)
    trainer.train()

    #f = open("result.txt", "a")
    #f.write("EXPIRE: FLOWID:{:d} PACKETCOUNT:{:d}\n".format(crc, packetCount))
    #f.close()
    #print packetCount
    #print rnn.activate(header)
    #print "Error: " + str(error)

def predict(crc, header):
    result = rnn.activate(header)[0]
    f = open("result.txt", "a")
    #print result
    f.write("INSERT: FLOWID:{:d} PRIORITY:{:f}\n".format(crc, result))
    f.close()
    return result
"""




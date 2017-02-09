ROX User Guide
==============

Building
--------
This project uses CMake to generate the makefile.  You must have cmake installed.

### Pre-requisites
libevent2
capnproto
CMake
g++
Python2.7
openvswitch

### Build Command
CMake .                  Generates the makefiles
make                     Builds the application

### Running The Program
There is a python program in NN/BrainServer.py which is the NN Microservice.  
Start the NN with python NN/BrainServer.py /location/for/data/

Execute the openflow controller with ./ROX

There is an simulation that does not require the use of the openflow controller
called NN/FFNNSimulation.py which will load data from /location/for/data, run
it through the FFNN, and calculate metrics for accuracy and calculate the
speedup.  Execute this program with python NN/FFNNSimulation.py /location/for/data/



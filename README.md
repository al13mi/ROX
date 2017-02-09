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

### Coverity Build
Coverity doesn't seem to work well with capnproto, so we strip out
the capnproto code before submitting a coverity build.

cmake . -DTYPE=COV  
~/cov-analysis-linux64-8.7.0/bin/cov-build --dir cov-int make

### Running The Program
There is a python program in NN/BrainServer.py which is the NN Microservice.  
Start the NN with python NN/BrainServer.py /location/for/data/

Execute the openflow controller with ./ROX

There is an simulation that does not require the use of the openflow controller
called NN/FFNNSimulation.py which will load data from /location/for/data, run
it through the FFNN, and calculate metrics for accuracy and calculate the
speedup.  Execute this program with python NN/FFNNSimulation.py /location/for/data/



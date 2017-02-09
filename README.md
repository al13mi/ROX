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

foo@debian:~/ROX$ cmake .
-- Found libevent2 includes:	/usr/include/event2/event.h
-- Found libevent2 library: /usr/lib/x86_64-linux-gnu/libevent.so
-- Configuring done
-- Generating done
-- Build files have been written to: /home/adtran/ROX
foo@debian:~/ROX$ make
Scanning dependencies of target Controller
[  2%] Building CXX object CMakeFiles/Controller.dir/Controller.cpp.o
[  5%] Linking CXX static library libController.a
[  5%] Built target Controller
[  7%] Building CXX object Network/CMakeFiles/Network.dir/ArpTable.cpp.o
[ 10%] Linking CXX static library libNetwork.a
[ 10%] Built target Network
[ 12%] Building CXX object OpenFlow/Messages/CMakeFiles/OxmTLV.dir/OxmTLV.cpp.o
[ 15%] Linking CXX static library libOxmTLV.a
[ 15%] Built target OxmTLV
[ 17%] Building CXX object OpenFlow/Messages/CMakeFiles/PacketInDecoder.dir/PacketInDecoder.cpp.o
[ 20%] Linking CXX static library libPacketInDecoder.a
[ 20%] Built target PacketInDecoder
[ 22%] Building CXX object OpenFlow/Messages/CMakeFiles/FlowModActionEncoder.dir/FlowModActionEncoder.cpp.o
[ 25%] Linking CXX static library libFlowModActionEncoder.a
[ 25%] Built target FlowModActionEncoder
[ 27%] Building CXX object OpenFlow/Messages/CMakeFiles/FlowMatchDecoder.dir/FlowMatchDecoder.cpp.o
[ 30%] Linking CXX static library libFlowMatchDecoder.a
[ 30%] Built target FlowMatchDecoder
[ 32%] Building CXX object OpenFlow/Messages/CMakeFiles/FlowMatchEncoder.dir/FlowMatchEncoder.cpp.o
[ 35%] Linking CXX static library libFlowMatchEncoder.a
[ 35%] Built target FlowMatchEncoder
[ 37%] Building CXX object OpenFlow/Messages/CMakeFiles/FlowModEncoder.dir/FlowModEncoder.cpp.o
[ 40%] Linking CXX static library libFlowModEncoder.a
[ 40%] Built target FlowModEncoder
[ 42%] Building CXX object OpenFlow/Messages/CMakeFiles/FlowRemovedDecoder.dir/FlowRemovedDecoder.cpp.o
[ 45%] Linking CXX static library libFlowRemovedDecoder.a
[ 45%] Built target FlowRemovedDecoder
[ 47%] Building CXX object OpenFlow/Messages/CMakeFiles/FlowModInstructionEncoder.dir/FlowModInstructionEncoder.cpp.o
[ 50%] Linking CXX static library libFlowModInstructionEncoder.a
[ 50%] Built target FlowModInstructionEncoder
[ 52%] Building CXX object OpenFlow/Messages/CMakeFiles/HeaderEncoder.dir/HeaderEncoder.cpp.o
[ 55%] Linking CXX static library libHeaderEncoder.a
[ 55%] Built target HeaderEncoder
[ 57%] Building CXX object OpenFlow/Messages/CMakeFiles/HeaderDecoder.dir/HeaderDecoder.cpp.o
[ 60%] Linking CXX static library libHeaderDecoder.a
[ 60%] Built target HeaderDecoder
[ 62%] Building CXX object OpenFlow/Messages/CMakeFiles/HelloDecoder.dir/HelloDecoder.cpp.o
[ 65%] Linking CXX static library libHelloDecoder.a
[ 65%] Built target HelloDecoder
[ 67%] Building CXX object OpenFlow/Messages/CMakeFiles/Messages.dir/FeaturesDecoder.cpp.o
[ 70%] Linking CXX static library libMessages.a
[ 70%] Built target Messages
Scanning dependencies of target OpenFlow
[ 72%] Building CXX object OpenFlow/CMakeFiles/OpenFlow.dir/OpenFlowTable.cpp.o
[ 75%] Linking CXX static library libOpenFlow.a
[ 75%] Built target OpenFlow
Scanning dependencies of target Brain
[ 77%] Building CXX object NN/CMakeFiles/Brain.dir/brain.capnp.cpp.o
[ 80%] Linking CXX static library libBrain.a
[ 80%] Built target Brain
Scanning dependencies of target NeuralNetwork
[ 82%] Building CXX object NN/CMakeFiles/NeuralNetwork.dir/NeuralNetwork.cpp.o
[ 85%] Linking CXX static library libNeuralNetwork.a
[ 85%] Built target NeuralNetwork
[ 87%] Building CXX object System/CMakeFiles/MacLayer.dir/MacLayer.cpp.o
[ 90%] Linking CXX static library libMacLayer.a
[ 90%] Built target MacLayer
[ 92%] Building CXX object System/CMakeFiles/System.dir/LookupTree.cpp.o
[ 95%] Linking CXX static library libSystem.a
[ 95%] Built target System
Scanning dependencies of target ROX
[ 97%] Building CXX object CMakeFiles/ROX.dir/main.cpp.o
[100%] Linking CXX executable ROX
[100%] Built target ROX

### Running The Program
There is a python program in NN/BrainServer.py which is the NN Microservice.  
Start the NN with python NN/BrainServer.py /location/for/data/

Execute the openflow controller with ./ROX

There is an simulation that does not require the use of the openflow controller
called NN/FFNNSimulation.py which will load data from /location/for/data, run
it through the FFNN, and calculate metrics for accuracy and calculate the
speedup.  Execute this program with python NN/FFNNSimulation.py /location/for/data/



OBJS=main.o
OBJS += Controller.o
OBJS += Network/ArpTable.o
OBJS += System/LookupTree.o
OBJS += OpenFlow/OpenFlowTable.o
OBJS += OpenFlow/Messages/HeaderEncoder.o
OBJS += OpenFlow/Messages/HeaderDecoder.o
OBJS += OpenFlow/Messages/HelloDecoder.o
OBJS += OpenFlow/Messages/FeaturesDecoder.o
OBJS += OpenFlow/Messages/FlowMatchEncoder.o
OBJS += OpenFlow/Messages/FlowMatchDecoder.o
OBJS += OpenFlow/Messages/OxmTLV.o
OBJS += OpenFlow/Messages/PacketInDecoder.o
OBJS += OpenFlow/Messages/FlowModEncoder.o
OBJS += OpenFlow/Messages/FlowModInstructionEncoder.o
OBJS += OpenFlow/Messages/FlowModActionEncoder.o
OBJS += OpenFlow/Messages/FlowRemovedDecoder.o
OBJS += NN/NeuralNetwork.o
OBJS += NN/brain.capnp.o


MAIN=controller
CFLAGS=-I$(realpath ../) -pthread -I. -g -msse4.2 -O0 -std=gnu++11 -L/usr/lib/x86_64-linux-gnu/ -L/usr/local/lib -I/usr/include/python2.7 -lpython2.7 -levent -fsanitize=address -pedantic -Wall -lcapnp -lcapnp-rpc -lkj -lkj-async
LDFLAGS= -g -levent -Wl,--whole-archive,-lrt,-lm,-ldl,--no-whole-archive -msse4.2 -O0 -std=gnu++11 -fsanitize=address -pedantic -L/usr/lib -lcapnp -lcapnp-rpc -lkj -lkj-async
LDFLAGS_TEST = -lgtest_main -lgtest -lgtest_main
CXX=g++
#CXX=clang++-3.8

.PHONY: all
all: $(MAIN)

.PHONY: upload
upload:
	$(RSYNC)

$(MAIN): $(OBJS)
	$(CXX) -o $(MAIN) $(OBJS) $(LDFLAGS) $(CFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CFLAGS)

t_%.o: %.cpp
	$(CXX) -o $@ -c $< $(CFLAGS) -DTESTMAIN

.PHONY: clean
clean:
	rm -f $(MAIN) $(LIB) $(OBJS) $(MAIN)_test t_$(OBJS)


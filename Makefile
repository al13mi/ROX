OBJS=main.o Controller.o Network/RouteTable.o Network/ArpTable.o System/LookupTree.o
TESTS = gtest/Test.o gtest/Router.o gtest/LookupTreeTest.o
MAIN=controller
CFLAGS=-I$(realpath ../) -pthread -I. -g -msse4.2 -O0 -std=gnu++11 -L/usr/lib/x86_64-linux-gnu/ -L/usr/local/lib -levent -fsanitize=address -pedantic -Wall
CFLAGS_TEST = -DTESTMAIN
LDFLAGS= -g -levent -Wl,--whole-archive,-lrt,-lm,-ldl,--no-whole-archive -msse4.2 -O0 -std=gnu++11 -fsanitize=address -pedantic -L/usr/lib
LDFLAGS_TEST = -lgtest_main -lgtest -lgtest_main
CXX=g++

RSYNC= rsync -avzr . adtran@10.17.150.9:~/ROX/

.PHONY: all
all: $(MAIN) $(TEST)

.PHONY: upload
upload:
	$(RSYNC)


.PHONY:
test: t_$(OBJS) $(TESTS)
	$(CXX) -o $(MAIN)_test $(TESTS) t_$(OBJS) $(LDFLAGS) $(LDFLAGS_TEST) $(CFLAGS)
	./$(MAIN)_test

$(MAIN): $(OBJS)
	$(CXX) -o $(MAIN) $(OBJS) $(LDFLAGS) $(CFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CFLAGS)

t_%.o: %.cpp
	$(CXX) -o $@ -c $< $(CFLAGS) -DTESTMAIN

.PHONY: clean
clean:
	rm -f $(MAIN) $(LIB) $(OBJS) $(MAIN)_test t_$(OBJS)
	rm -f $(TESTS)


OBJS=main.o Controller.o
MAIN=controller
CFLAGS=-I$(realpath ../) -pthread -I. -g -msse4.2 -O0 -std=gnu++11 -L/usr/lib/x86_64-linux-gnu/ -levent -fsanitize=address -pedantic -Wall
LDFLAGS= -g -levent -Wl,--whole-archive,-lrt,-lm,-ldl,--no-whole-archive -msse4.2 -O0 -std=gnu++11 -fsanitize=address -pedantic
CXX=g++

.PHONY: all clean

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CXX) -o $(MAIN) $(OBJS) $(LDFLAGS) $(CFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CFLAGS)

clean:
	rm -f $(MAIN) $(LIB) $(OBJS)

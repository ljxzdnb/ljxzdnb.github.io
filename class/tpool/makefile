#CC = g++ 
#CompileOpt = -g -std=gnu++11
#LinkOpt = -lpthread  $(asan)
#
#Target = main
#
#$(Target) : main.o memory_leak.o
#    $(CC) main.o memory_leak.o -o $(Target) $(LinkOpt)
#    $(info info text)
#    $(warning warn text)
#    $(warning $(asan))
#    $(error error text)
#all:
#	$(CC) -c $(CompileOpt) main.cpp

#clean:
#	rm main.o memory_leak.o

.PHONY: all  clean
SRC=$(wildcard *.c)
OBJ=$(SRC:%.c=%.o)
BIN=$(SRC:%.c=%)
#ARC=$(wildcard *.o)

LINK=$(CROSS_COMPILE)ar cr
CC=$(CROSS_COMPILE)gcc
CFLAGS=-Wall
target=tar.a
#target:$(ARC)
#	$(CC) $^ $(CFLAGS) -o $(target)

all:$(BIN)
$(BIN):%:%.o
	$(CC) $^ $(CFLAGS) -o $@

clean:
	rm $(OBJ) $(BIN)

#.PHONY:$(ALL) clean
PAT =  mpool/ ring/ tpool/ table/ sock/ react/ heap/ ./
CPT := $(shell pwd)
SRC := $(foreach pat,$(PAT),$(wildcard $(pat)*.c))
INC := $(dir $(SRC))
OBJ := $(SRC:%.c=%.o)
BIN := $(SRC:%.c=%)
ALL = main
CFLAGS = -D__DEBUG__ -D__EVSIZE__=50 -g
LIB = -lpthread
CC = $(CROSS_COMPILE)gcc
$(ALL):$(OBJ)
	$(CC) $(CFLAGS) $(LIB) -o $@ $^
clean:
	rm $(BIN) $(OBJ)

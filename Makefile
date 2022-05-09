# Makefile for Multithreaded Factorial Calculator in C

# -Wno-comment: disable warnings for multi-line comments, present in some tests
CFLAGS      = -Wall -Wno-comment -Werror -g
CFLAGS_FAST = -Ofast
LIBS        = -lgmp -lpthread
CC_TEST     = gcc $(CFLAGS)
CC          = gcc $(CFLAGS_FAST)
SHELL       = /bin/bash
CWD         = $(shell pwd | sed 's/.*\///g')

PROGRAMS = \
	factorial\

all : $(PROGRAMS)

clean : clean-execs clean-results

debug : factorial-debug

clean-execs :
	rm -f factorial-debug *.o $(PROGRAMS)

factorial-debug.o : factorial.c
	$(CC_TEST) -o $@ -c $^

factorial-debug : factorial-debug.o
	$(CC_TEST) -o $@ $^ $(LIBS)

factorial.o : factorial.c
	$(CC) -c $<

factorial : factorial.o
	$(CC) -o $@ $^ $(LIBS)

clean-results :
	rm -f output.txt

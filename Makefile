CFLAGS=-Wall -g -fdiagnostics-color=always

build: stack.o scalc.o
	gcc $(CFLAGS) -o scalc stack.o scalc.o main.c
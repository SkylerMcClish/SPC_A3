CC = gcc

TARGET = defrag

all:
	gcc -g -Wall -Werror -std=gnu11 -o defrag defrag.c -lpthread


test: $(TARGET).c
	
	gcc -g -o defrag defrag.c -lpthread

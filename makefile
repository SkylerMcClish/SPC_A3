CC = gcc

TARGET = defrag

all:
	gcc -g -Wall -Werror -std=gnu11 -o -lpthread defrag defrag.c


test: $(TARGET).c
	
	gcc -g -o -lpthread defrag defrag.c

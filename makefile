CC = gcc

TARGET = defrag

all:
	gcc -g -Wall -Werror -std=gnu11 -o -lpthread $(TARGET) $(TARGET).c


test: $(TARGET).c
	$(CC) -g -o $(TARGET) $(TARGET).c
	gcc -g -o -lpthread defrag defrag.c

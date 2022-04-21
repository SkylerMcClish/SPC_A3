CC = gcc

TARGET = defrag

all:
	gcc -g -Wall -Werror -std=gnu11 -o $(TARGET) $(TARGET).c


test: $(TARGET).c
	$(CC) -g -o $(TARGET) $(TARGET).c
	gcc -g -o defrag defrag.c

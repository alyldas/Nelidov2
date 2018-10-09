CC = gcc
CFLAGS = -c -Wall

all: Nelidov2
	
Nelidov2: functions.o main.o
	$(CC) main.c -o Nelidov2

main.o: main.c
	$(CC) $(CFLAGS) main.c

functions.o: functions.c
	$(CC) $(CFLAGS) functions.c

clean:
	rm Nelidov2
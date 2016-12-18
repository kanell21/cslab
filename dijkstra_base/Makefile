C = gcc
CFLAGS = -Wall -Wextra -g -O3 -std=c99
LDFLAGS = -pthread -lm

all: dijkstra_serial dijkstra_serial_uni

## SERIAL
dijkstra_serial: test_dijkstra_serial.o dijkstra_serial.o minheap.o graph.o
	$(CC) $(LDFLAGS) -mrtm -o dijkstra_serial test_dijkstra_serial.o dijkstra_serial.o minheap.o graph.o

test_dijkstra_serial.o: test_dijkstra.c
	$(CC) $(CFLAGS) -DTOTAL -c -o test_dijkstra_serial.o test_dijkstra.c

dijkstra_serial.o: dijkstra.c
	$(CC) $(CFLAGS) -mrtm -c -o dijkstra_serial.o dijkstra.c

## SERIAL UNI
dijkstra_serial_uni: test_dijkstra_serial_uni.o dijkstra_serial_uni.o minheap.o graph.o
	$(CC) $(LDFLAGS) -mrtm -o dijkstra_serial_uni test_dijkstra_serial_uni.o dijkstra_serial_uni.o minheap.o graph.o

test_dijkstra_serial_uni.o: test_dijkstra.c
	$(CC) $(CFLAGS) -DTOTAL_UNI -c -o test_dijkstra_serial_uni.o test_dijkstra.c

dijkstra_serial_uni.o: dijkstra.c
	$(CC) $(CFLAGS) -mrtm -c -o dijkstra_serial_uni.o dijkstra.c


## EXTRA
dijkstra.o: dijkstra.c 
	$(CC) $(CFLAGS) -mrtm -c -o dijkstra.o dijkstra.c

minheap.o: minheap.c minheap.h
	$(CC) $(CFLAGS) -c -o minheap.o minheap.c 

graph.o: graph.c graph.h minheap.h
	$(CC) $(CFLAGS) -c -o graph.o graph.c

transaction.o: transaction.c transaction.h
	$(CC) $(CFLAGS) -mrtm -c -o transaction.o transaction.c 

clean:
	rm -f *.o  dijkstra_serial dijkstra_serial_uni

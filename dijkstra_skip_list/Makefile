CC=gcc
CFLAGS=-Wall -O3 -std=c99 
LDFLAGS=-lm

all:dijkstra_skiplist_total dijkstra_skiplist_extract dijkstra_skiplist_update dijkstra_skiplist_decrease dijkstra_skiplist_insert

dijkstra_skiplist_total:dijkstra_skiplist_total.o sl_pq.o util.o
	$(CC) $(CFLAGS) -DTOTAL  -o dijkstra_skiplist_total sl_pq.o util.o dijkstra_skiplist_total.o -lm
dijkstra_skiplist_total.o:dijkstra_skiplist.c
	$(CC) $(CFLAGS)-DTOTAL -o dijkstra_skiplist_total.o -c dijkstra_skiplist.c


dijkstra_skiplist_extract:dijkstra_skiplist_extract.o sl_pq.o util.o
	$(CC) $(CFLAGS) -DEXTRACT  -o dijkstra_skiplist_extract sl_pq.o util.o dijkstra_skiplist_extract.o -lm
dijkstra_skiplist_extract.o:dijkstra_skiplist.c
	$(CC) $(CFLAGS)-DEXTRACT -o  dijkstra_skiplist_extract.o -c dijkstra_skiplist.c


dijkstra_skiplist_update:dijkstra_skiplist_update.o sl_pq.o util.o
	$(CC) $(CFLAGS) -DUPDATE  -o dijkstra_skiplist_update sl_pq.o util.o dijkstra_skiplist_update.o -lm
dijkstra_skiplist_update.o:dijkstra_skiplist.c
	$(CC) $(CFLAGS)-DUPDATE -o dijkstra_skiplist_update.o -c dijkstra_skiplist.c


dijkstra_skiplist_decrease:dijkstra_skiplist_decrease.o sl_pq.o util.o
	$(CC) $(CFLAGS) -DDECREASE  -o dijkstra_skiplist_decrease sl_pq.o util.o dijkstra_skiplist_decrease.o -lm
dijkstra_skiplist_decrease.o:dijkstra_skiplist.c
	$(CC) $(CFLAGS)-DDECREASE -o dijkstra_skiplist_decrease.o -c dijkstra_skiplist.c


dijkstra_skiplist_insert:dijkstra_skiplist_insert.o sl_pq.o util.o
	$(CC) $(CFLAGS) -DINSERT  -o dijkstra_skiplist_insert sl_pq.o util.o dijkstra_skiplist_insert.o -lm
dijkstra_skiplist_insert.o:dijkstra_skiplist.c
	$(CC) $(CFLAGS)-DINSERT  -o dijkstra_skiplist_insert.o -c dijkstra_skiplist.c



sl_pq.o: sl_pq.c sl_pq.h
	$(CC) $(CFLAGS) -c -o sl_pq.o sl_pq.c -lm

util.o:util.c util.h
	$(CC) $(CFLAGS) -c -o util.o util.c


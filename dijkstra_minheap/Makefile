#
# Makefile for Heaps

CC=gcc

timers: dijkstra_minheap_timers_total dijkstra_minheap_timers_update dijkstra_minheap_timers_decrease dijkstra_minheap_timers_extract dijkstra_minheap_timers_decrease dijkstra_minheap_timers_insert

dijkstra_minheap.o: dijkstra_minheap.c
	$(CC) -Wall -O3 -std=c99  -c dijkstra_minheap.c

dijkstra_minheap: dijkstra_minheap.o
	$(CC) -O3 -o dijkstra_minheap  dijkstra_minheap.o 
dijkstra_minheap_timers_total: dijkstra_minheap_timers.c
	$(CC) -Wall -DTOTAL -std=c99 -O3 -o dijkstra_minheap_timers_total  dijkstra_minheap_timers.c
dijkstra_minheap_timers_update: dijkstra_minheap_timers.c
	$(CC) -Wall -DUPDATE -std=c99 -O3 -o dijkstra_minheap_timers_update  dijkstra_minheap_timers.c
dijkstra_minheap_timers_insert: dijkstra_minheap_timers.c
	$(CC) -Wall -DINSERT -std=c99 -O3 -o dijkstra_minheap_timers_insert  dijkstra_minheap_timers.c
dijkstra_minheap_timers_decrease: dijkstra_minheap_timers.c
	$(CC) -Wall -DDECREASE -std=c99 -O3 -o dijkstra_minheap_timers_decrease  dijkstra_minheap_timers.c
dijkstra_minheap_timers_extract: dijkstra_minheap_timers.c
	$(CC) -Wall -DEXTRACT -std=c99 -O3 -o dijkstra_minheap_timers_extract  dijkstra_minheap_timers.c


clean: 
	rm dijkstra_minheap *.o
	

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
dijkstra_minheap_prev: dijkstra_minheap_prev.c
	$(CC) -Wall -O3 -std=c99 -DTOTAL dijkstra_minheap_prev.c  -o dijkstra_minheap_prev
dijkstra_minheap_optim: dijkstra_minheap_optim.c
	$(CC) -Wall -O3 -std=c99 -DTOTAL dijkstra_minheap_optim.c  -o dijkstra_minheap_optim
dijkstra_minheap_adjarr_optim: dijkstra_minheap_adjarr_optim.c
	$(CC)  -Wall -O3 -std=c99  -DUPDATE dijkstra_minheap_adjarr_optim.c  -o dijkstra_minheap_adjarr_optim
dijkstra_minheap_fullprefetch:dijkstra_minheap_fullprefetch.c
	$(CC)  -Wall -O3 -std=c99  -DUPDATE dijkstra_minheap_fullprefetch.c  -o dijkstra_minheap_fullprefetch
dijkstra_minheap_fullprefetch_clever:dijkstra_minheap_fullprefetch_clever.c
	$(CC)  -Wall -O3 -std=c99  -DUPDATE dijkstra_minheap_fullprefetch_clever.c  -o dijkstra_minheap_fullprefetch_clever
dijkstra_minheap_2sym:dijkstra_minheap_2sym.c
	$(CC)  -Wall -O3 -std=c99 -fopenmp -DUPDATE dijkstra_minheap_2sym.c  -o dijkstra_minheap_2sym
dijkstra_minheap_2sym_transact:dijkstra_minheap_2sym_transact.c transaction.o
	$(CC)  -Wall -g -O3 -mrtm -std=c99 -fopenmp -DUPDATE dijkstra_minheap_2sym_transact.c transaction.o  -o dijkstra_minheap_2sym_transact
dijkstra_minheap_adjarr_uni_optim: dijkstra_minheap_adjarr_uni_optim.c
	$(CC)  -Wall  -std=c99 -fprefetch-loop-arrays -DTOTAL dijkstra_minheap_adjarr_uni_optim.c  -o dijkstra_minheap_adjarr_uni_optim
dijkstra_minheap_optim_noprev: dijkstra_minheap_optim.c
	$(CC) -Wall -O3 -std=c99 -DTOTAL dijkstra_minheap_optim_noprev.c  -o dijkstra_minheap_optim_noprev
dijkstra_minheap_prefetch: dijkstra_minheap_prefetch.c
	$(CC) $(Flags) -std=c99 -fprefetch-loop-arrays -DTOTAL dijkstra_minheap_prefetch.c  -o dijkstra_minheap_prefetch












transaction.o:transaction.c
	gcc -Wall -g -fopenmp -mrtm -c transaction.c



clean: 
	rm dijkstra_minheap *.o
	

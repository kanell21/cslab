

CC=gcc
timers: dijkstra_fibo_timers_total dijkstra_fibo_timers_extract dijkstra_fibo_timers_update dijkstra_fibo_timers_decrease dijkstra_fibo_timers_insert

dijkstra_fibo: dijkstra_fiboheapnew.c
	$(CC) -Wall -g -O3 -o dijkstra_fibo dijkstra_fiboheapnew.c -lm

dijkstra_fibo_timers_total: dijkstra_fiboheapnew_timers.c
	$(CC) -Wall -g -O3 -DTOTAL -o dijkstra_fibo_timers_total dijkstra_fiboheapnew_timers.c -lm

dijkstra_fibo_count: dijkstra_fiboheapnew_nb.c
	$(CC) -Wall -g -O3 -DTOTAL -o dijkstra_fibo_count dijkstra_fiboheapnew_nb.c -lm
dijkstra_fibo_timers_extract: dijkstra_fiboheapnew_timers.c
	$(CC) -Wall -g -O3 -DEXTRACT -o dijkstra_fibo_timers_extract dijkstra_fiboheapnew_timers.c -lm
dijkstra_fibo_timers_extractf: dijkstra_fiboheapnew_timers.c
	$(CC) -Wall -g -O3 -DEXTRACT_FIRST -o dijkstra_fibo_timers_extractf dijkstra_fiboheapnew_timers.c -lm
dijkstra_fibo_timers_update: dijkstra_fiboheapnew_timers.c
	$(CC) -Wall -g -O3 -DUPDATE -o dijkstra_fibo_timers_update dijkstra_fiboheapnew_timers.c -lm

dijkstra_fibo_timers_decrease: dijkstra_fiboheapnew_timers.c
	$(CC) -Wall -g -O3 -DDECREASE -o dijkstra_fibo_timers_decrease dijkstra_fiboheapnew_timers.c -lm

dijkstra_fibo_timers_insert: dijkstra_fiboheapnew_timers.c
	$(CC) -Wall -g -O3 -DINSERT -o dijkstra_fibo_timers_insert dijkstra_fiboheapnew_timers.c -lm

CC=gcc
% :: %.c
	$(CC) -g -Wall -Wvla -Werror $< -o $@ -lpthread -lm

all: thread_mandelbrot  buddhabrot search

thread_mandelbrot: thread_mandelbrot.c read_ppm.c write_ppm.c
	$(CC) -g -Wall -Wvla -Werror -Wno-unused-variable thread_mandelbrot.c read_ppm.c write_ppm.c -o $@ -lm -lpthread

buddhabrot: buddhabrot.c read_ppm.c write_ppm.c
	$(CC) -g -Wall -Wvla -Werror -Wno-unused-variable buddhabrot.c read_ppm.c write_ppm.c -o $@ -lm -lpthread

clean: 
	rm thread_mandelbrot buddhabrot search


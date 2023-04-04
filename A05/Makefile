CC=gcc
% :: %.c
	$(CC) -g -Wall -Wvla -Werror $< -o $@

all : crossword test_read test_write

test_read :: test_read.c read_ppm.c 
	$(CC) -g -Wall -Wvla -Werror $< read_ppm.c -o $@

test_write :: test_write.c read_ppm.c write_ppm.c
	$(CC) -g -Wall -Wvla -Werror $< read_ppm.c write_ppm.c -o $@

clean :
	rm crossword test_read test_write 2> /dev/null

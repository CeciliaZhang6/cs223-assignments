CC=gcc
% :: %.c
	$(CC) -g -Wall -Wvla -Werror $< -o $@

all : dynamic_songs slow

clean :
	rm dynamic_songs slow 2> /dev/null

CC=gcc
% :: %.c
	$(CC) -g -Wall -Wvla -Werror $< -o $@

encode : encode.c read_ppm.c write_ppm.c
	$(CC) -g -Wall -Wvla -Werror encode.c read_ppm.c write_ppm.c -o $@

decode : decode.c read_ppm.c write_ppm.c
	$(CC) -g -Wall -Wvla -Werror decode.c read_ppm.c write_ppm.c -o $@

all: encode decode bitmap

clean :
	rm encode decode bitmap 2> /dev/null

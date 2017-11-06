CC = gcc
CFLAGS = -g -O0 -Wall -Wextra

all: read_test parse_test

read_test: read_file.o
	gcc $(CFLAGS) read_file.o read_test.c -o read_test

parse_test: read_file.o parse.o
	gcc $(CFLAGS) read_file.o parse.o parse_test.c -o parse_test

test: parse_test
	@sh runtests.sh

clean:
	rm -f read_test parse_test read_file.o parse.o

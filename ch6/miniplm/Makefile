CC = gcc
CFLAGS = -g -O0 -Wall -Wextra

all: read_test parse_test fill_test

read_test: read_file.o print.o
	$(CC) $(CFLAGS) $^ read_test.c -o read_test

parse_test: read_file.o parse.o print.o
	$(CC) $(CFLAGS) $^ parse_test.c -o parse_test

fill_test: read_ast.o vardata.o parse.o read_file.o
	$(CC) $(CFLAGS) $^ fill_test.c -o fill_test

test: parse_test
	@sh runtests.sh

clean:
	rm -f read_test parse_test fill_test *.o

exec = ocean
files = $(wildcard src/*.c)
CC = gcc
flags = -g

compile:
	$(CC) $(flags) $(files) -o $(exec)

wall:
	$(CC) $(flags) -Wall $(files) -o $(exec)

run:
	./ocean examples/variable.ocn

all: compile run

clean:
	rm ocean

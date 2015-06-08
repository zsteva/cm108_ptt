all: test.c cm108.c
	gcc -o test test.c cm108.c -I.

.PHONY: all

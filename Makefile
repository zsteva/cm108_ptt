all: test.c cm108.c
	gcc -ggdb -o test test.c cm108.c -ludev -I.

.PHONY: all

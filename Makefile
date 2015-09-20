all: test.c cm108.c
	gcc -o test test.c cm108.c -ludev -I.

.PHONY: all

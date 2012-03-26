#
# Author: Heri
# 
# CC=gcc|suncc|icc
#
CC=gcc

CFLAGS_gcc="-O5"
CFLAGS_suncc=""
CFLAGS_icc=""

compile:
	$(CC) -o hello_$(CC) hello_world.c
run:
	./hello_$(CC)
clean:
	rm -Rf hello_$(CC)

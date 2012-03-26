all:build

build:clean
	gcc -Wall main.c -o main

run:build
	./main

clean:
	rm -f main

.PHONY: all build run

EXE = main

run: build
	./$(EXE)

build: $(EXE)

main: main.c
	gcc -o main main.c


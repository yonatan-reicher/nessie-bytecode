.PHONY: all build run clean

SOURCES = $(wildcard *.c)

run: build
	./bin/main

build: bin/main

clean:
	rm -r bin/

bin:
	mkdir -p bin

bin/main: $(SOURCES)
	gcc -o bin/main main.c

opcode.c: generate_opcode.c bin
	gcc -o bin/generate_opcode generate_opcode.c
	./bin/generate_opcode > opcode.c

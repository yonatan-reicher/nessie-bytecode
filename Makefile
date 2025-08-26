.PHONY: all build run clean

run: build
	./bin/main

build: bin/main

clean:
	rm -r bin/ opcode.c

bin:
	mkdir -p bin

bin/main: main.c opcode.c bin
	gcc -o bin/main main.c

opcode.c: generate_opcode.c bin
	gcc -o bin/generate_opcode generate_opcode.c
	./bin/generate_opcode > opcode.c

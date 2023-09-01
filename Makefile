CC := g++
CCARGS := -Wall -Werror -Wpedantic -g

.PHONY: clean
all: clean compile run test

compile:
	$(CC) src/*.cpp -o build/main -I./src/include $(CCARGS)

run:
	./build/main example/main.ocn

bear:
	bear -- make

clean:
	rm -rf build
	mkdir build

debug:
	gdb --args ./build/main example/main.ocn

testCode:
	./test/testFile

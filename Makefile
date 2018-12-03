run-native: sudoku-gen
	./sudoku-gen 100

# build linux binary
sudoku-gen: main.o sudoku-gen.o
	g++ -std=c++1z $? -o sudoku-gen

# build all object files for linux
%.o: %.cpp
	g++ -std=c++1z -O3 -c $< -o $@


run-wasm: pkg
	node hello.js 100

# Build wasm package
pkg: sudoku-gen.cpp sudoku-gen.h Makefile
	mkdir -p pkg
	emcc -std=c++1z -Wall \
		-o pkg/sudoku-gen.js sudoku-gen.cpp \
		--bind \
		-s WASM=1 -s MODULARIZE=1 \
		-DWASM=true

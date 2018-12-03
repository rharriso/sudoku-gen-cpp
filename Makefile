# build linux binary
sudoku-gen: main.o sudoku-gen.o
	clang++ -std=c++1z main.o $< $@

# build all object files for linux
%.o: %.cpp
	clang++ -std=c++1z -O3 -c $< -o $@

run-wasm: pkg
	node hello.js 100
		
# Build wasm package
pkg:
	mkdir -p pkg
	emcc --bind -o pkg/sudoku-gen.js -std=c++1z main.cpp -s WASM=1 -Wall -s MODULARIZE=1


sudoku-gen: main.o
	clang++ -std=c++1z main.o -o sudoku-gen

%.o: %.cpp
	clang++ -std=c++1z -O3 -c $< -o $@

run-asm:
	emcc -std=c++1z -O3 main.cpp
	python -m SimpleHTTPServer 8080

run-wasm:
	emcc -std=c++1z -O3 -s WASM=1 main.cpp
	python -m SimpleHTTPServer 8080
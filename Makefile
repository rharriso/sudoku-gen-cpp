sudoku-gen: main.cpp
	clang++  -std=c++1z -O3 main.cpp -o sudoku-gen

run-asm:
	emcc -std=c++1z -O3 main.cpp
	python -m SimpleHTTPServer 8080

run-wasm:
	emcc -std=c++1z -O3 -s WASM=1 main.cpp
	python -m SimpleHTTPServer 8080
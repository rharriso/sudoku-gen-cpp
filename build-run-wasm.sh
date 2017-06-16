#!/bin/bash

emcc -std=c++1z -O3 -s WASM=1 main.cpp

echo ""
echo "open http://localhost:8080/out.html"
echo ""
echo ""

python -m SimpleHTTPServer 8080


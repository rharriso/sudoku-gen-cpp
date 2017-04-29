# sudoku-gen-cpp
sudoku gen cpp

## Building And Running
```sh
mkdir build
cd build
cmake .. && make
./sudoku_generator_cpp 2 --all-neighbors
```
## Usage

```
sudoku_generator_cpp board_count [--all-neighbors]
# board_count: number of boards to generate
# --all-neighbors: set to use all neighbors when setting values (not just optimal set)
```

#include <set>
#include <deque>
#include <memory>
#include <sstream>

struct coord {
    int i = 0;
    int j = 0;
};


class SudokuCell {
public:
    coord pos;
    std::set<coord> neighbors{}; // neighbors to look at when filling from top-left to bottom right
    int value = 0;

    SudokuCell() {}
    void setPosition(coord pos);

private:
    void generateOptimalNeighbors ();
};

using cellQueue = std::deque<std::shared_ptr<SudokuCell>>;


class SudokuBoard {
    cellQueue cells;

public:
    SudokuBoard();
    void fillCells();
    std::string serialize();

private:
    bool doFillCells(int index);
    std::shared_ptr<SudokuCell> at(int index);
    std::shared_ptr<SudokuCell> at(coord position);
};

SudokuBoard generateAndFillBoard();
void generateAndFillBoards(int numBoards);


#ifdef WASM
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(sudoku_gen) {
  emscripten::function("generateAndFillBoard", &generateAndFillBoard);
  emscripten::function("generateAndFillBoards", &generateAndFillBoards);

  emscripten::class_<SudokuBoard>("SudokuBoard")
    .function("serialize", &SudokuBoard::serialize);
}
#endif
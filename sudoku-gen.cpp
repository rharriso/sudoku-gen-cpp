#include "sudoku-gen.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

constexpr int SIZE = 9;
constexpr int THIRD = SIZE / 3;
const std::set<int> VALID_VALUES = {1, 2, 3, 4, 5, 6, 7, 8, 9};
std::random_device rd;
std::mt19937 randomGen(rd());

inline bool operator == (const coord &lhs, const coord &rhs) {
    return lhs.i == rhs.i && lhs.j == rhs.j;
}

inline bool operator < (const coord &lhs, const coord &rhs) {
    return lhs.i < rhs.i || (lhs.i == rhs.i && lhs.j < rhs.j);
}

void SudokuCell::setPosition(coord pos) {
    this->pos = pos;
    generateOptimalNeighbors();
}

void SudokuCell::generateOptimalNeighbors () {
    // generate row neighbors
    for (int i = 0; i < pos.i; ++i) {
        neighbors.insert({i, pos.j});
    }

    // generate col neighbors
    for (int j = 0; j < pos.j; ++j) {
        neighbors.insert({pos.i, j});
    }

    auto iFloor = (pos.i / THIRD) * THIRD;
    auto jFloor = (pos.j / THIRD) * THIRD;

    // generate block neighbors
    for (int i = iFloor; i <= pos.i ; ++i) {
        for (int j = jFloor; (i < pos.i && j < jFloor + THIRD) || j < pos.j ; ++j) {
            neighbors.insert({i, j});
        }
    }
}

/**
 * take index and return 2d coord
 */
coord resolveIndex(int index) {
    return coord{
        index / SIZE,
        index % SIZE
    };
}

/**
 * Take coord and return index
 */
int resolvePosition(const coord &position) {
    return position.i * SIZE + position.j;
}

SudokuBoard::SudokuBoard() {
    for(int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            auto cell = std::make_shared<SudokuCell>(SudokuCell());
            cell->setPosition({i, j});
            cells.push_back(cell);
        }
    }
}

/**
 * fill the board with valid solution
 **/
void SudokuBoard::fillCells() {
    if(!doFillCells(0)) {
        std::cerr << "Unable to fill board" << '\n';
    }
}

bool SudokuBoard::doFillCells(int index) {
    // get first cell and tail
    auto cell = cells.at(index);

    std::set<int> neighborValues = {};

    for(auto &neighbor : cell->neighbors) {
        auto value = this->at(neighbor)->value;
        neighborValues.insert(value);
    }

    std::vector<int> options;
    set_difference(
            VALID_VALUES.begin(), VALID_VALUES.end(),
            neighborValues.begin(), neighborValues.end(),
            std::inserter(options, options.begin())
    );
    shuffle(options.begin(), options.end(), randomGen);

    for(auto option : options) {
        cell->value = option;

        if (index == cells.size() - 1 || doFillCells(index + 1)) {
            return true;
        }
    }

    // out of options backtrack
    cell->value = 0;
    return false;
}

/**
 * serialize a board
 */
std::string SudokuBoard::serialize() {
    std::stringstream ostream;

    for(auto cell: cells) {
        ostream << cell->value << '|';
    }

    return ostream.str();
}

/**
 *
 */
std::shared_ptr<SudokuCell> SudokuBoard::at(int index) {
    return this->cells.at(index);
}

/**
 * Get cell by position
 */
std::shared_ptr<SudokuCell> SudokuBoard::at(coord position) {
    auto index = resolvePosition(position);
    return this->at(index);
}

SudokuBoard generateAndFillBoard() {
    SudokuBoard board{};
    board.fillCells();
    return board;
}

void generateAndFillBoards(int numBoards) {
  for(int i = 0; i < numBoards; i++) {
    SudokuBoard board{};
    board.fillCells();
  }
}
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <experimental/random>
#include <stdlib.h>
#include <deque>
#include <memory>

using namespace std;

constexpr int SIZE = 9;
constexpr int THIRD = SIZE / 3;
const vector<int> ZERO_THRU_EIGHT = {0, 1, 2, 3, 4, 5, 6, 7, 8};
const set<int> VALID_VALUES = {1, 2, 3, 4, 5, 6, 7, 8, 9};

struct coord {
    int i = 0;
    int j = 0;
};

inline bool operator == (const coord &lhs, const coord &rhs) {
    return lhs.i == rhs.i && lhs.j == rhs.j;
}

inline bool operator < (const coord &lhs, const coord &rhs) {
    return lhs.i < rhs.i || (lhs.i == rhs.i && lhs.j < rhs.j);
}


class SudokuCell {
public:
    coord pos;
    set<coord> neighbors{};
    int value = 0;

    SudokuCell() {}

    void setPosition(coord pos) {
        this->pos = pos;
        
        // generate row neighbors
        for (auto j : ZERO_THRU_EIGHT) {
            neighbors.insert({pos.i, j});
        }

        // generate col neighbors
        for (auto i : ZERO_THRU_EIGHT) {
            neighbors.insert({i, pos.j});
        }

        auto iFloor = (pos.i / THIRD) * THIRD;
        auto jFloor = (pos.j / THIRD) * THIRD;

        // generate cell neighbors
        for (int i = iFloor; i < iFloor + THIRD; ++i) {
            for (int j = jFloor; j < jFloor + THIRD; ++j) {
                neighbors.insert({i, j});
            }
        }

        neighbors.erase(pos);
    }
};

using cellQueue = deque<shared_ptr<SudokuCell>>;

class SudokuBoard {
    cellQueue cells;

public:

    SudokuBoard() {
        for(int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                auto cell = make_shared<SudokuCell>(SudokuCell());
                cell->setPosition({i, j});
                cells.push_back(cell);
            }
        }
    }

    /*
     * fill the board with valid solution
     */
    void fillCells() {
        auto remaininCells = cellQueue{cells};

        if(!doFillCells(remaininCells)) {
            cout << "Unable to fill board" << endl;
        }
    }

    bool doFillCells(cellQueue &remainingCells) {
        // get first cell and tail
        auto cell = remainingCells.front();
        remainingCells.pop_front();

        set<int> neighborValues = {};

        for(auto &neighbor : cell->neighbors) {
            auto value = this->at(neighbor)->value;
            neighborValues.insert(value);
        }

        vector<int> options;
        set_difference(
                VALID_VALUES.begin(), VALID_VALUES.end(),
                neighborValues.begin(), neighborValues.end(),
                inserter(options, options.begin())
        );
        random_shuffle(options.begin(), options.end());


        for(auto option : options) {
            cell->value = option;

            if (remainingCells.empty() || doFillCells(remainingCells)) {
                return true;
            }
        }

        // out of options backtrack
        cell->value = 0;
        remainingCells.push_front(cell);
        return false;
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

    /**
     *
     */
    shared_ptr<SudokuCell> at(int index) {
        return this->cells.at(index);
    }

    /**
     *
     */
    shared_ptr<SudokuCell> at(coord position) {
        auto index = this->resolvePosition(position);
        return this->at(index);
    }

    /**
     * print
     */
    void printBoard() {
        cout << endl << "----------------------------------" << endl;

        for(int i = 0; i < SIZE; i++) {
            cout << "| ";

            for(int j = 0; j < SIZE; j++) {
                auto index = resolvePosition({i, j}); // YOOOO structs
                auto cell = cells[index];

                cout << cell->value << "  ";

                if (j % THIRD == THIRD - 1) {
                    cout << "| ";
                }
            }
            if (i % THIRD == THIRD - 1) {
                cout << endl << "----------------------------------";
            }
            cout << endl;
        }
    }
};


int main() {
    srand (time(NULL));
    SudokuBoard board{};

    auto cell = board.at({5, 3});
    
    board.fillCells();
    board.printBoard();

    return 0;
}

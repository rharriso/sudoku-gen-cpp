#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <stdlib.h>
#include <deque>
#include <memory>
#include <sstream>
#include <cstring>
#include <chrono>

using namespace std;

constexpr int SIZE = 9;
constexpr int THIRD = SIZE / 3;
const set<int> VALID_VALUES = {1, 2, 3, 4, 5, 6, 7, 8, 9};
bool g_allNeighbors = false;

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
    set<coord> fillNeighors{}; // neighbors to look at when filling from top-left to bottom right
    int value = 0;

    SudokuCell() {}

    void setPosition(coord pos) {
        this->pos = pos;

        // generate neighbors
        (g_allNeighbors) ? generateAllNeighbors() :
                           generateOptimalNeighbors();
    }

    void generateAllNeighbors () {
        // generate row fillNeighors
        for (int i = 0; i < SIZE; ++i) {
            fillNeighors.insert({i, pos.j});
        }

        // generate col fillNeighors
        for (int j = 0; j < SIZE; ++j) {
            fillNeighors.insert({pos.i, j});
        }

        auto iFloor = (pos.i / THIRD) * THIRD;
        auto jFloor = (pos.j / THIRD) * THIRD;

        // generate cell fillNeighors
        for (int i = iFloor; i < iFloor + THIRD ; ++i) {
            for (int j = jFloor; j < jFloor + THIRD; ++j) {
                fillNeighors.insert({i, j});
            }
        }
    }

    void generateOptimalNeighbors () {
        // generate row fillNeighors
        for (int i = 0; i < pos.i; ++i) {
            fillNeighors.insert({i, pos.j});
        }

        // generate col fillNeighors
        for (int j = 0; j < pos.j; ++j) {
            fillNeighors.insert({pos.i, j});
        }

        auto iFloor = (pos.i / THIRD) * THIRD;
        auto jFloor = (pos.j / THIRD) * THIRD;

        // generate cell fillNeighors
        for (int i = iFloor; i <= pos.i ; ++i) {
            for (int j = jFloor; (i < pos.i && j < jFloor + THIRD) || j < pos.j ; ++j) {
                fillNeighors.insert({i, j});
            }
        }
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

    void reset () {
        for(auto cell : cells) {
            cell->value = 0;
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

        for(auto &neighbor : cell->fillNeighors) {
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
                auto value = cell->value;

                if ( value > 0 ) {
                  cout << cell->value << "  ";
                } else {
                  cout << "   ";
                }

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

    /**
     * serialize
     */
    string serialize() {
        stringstream ostream;

        for(auto cell: cells) {
            ostream << cell->value;
        }

        return ostream.str();
    }
};

void printUsageError(char *programName) {
    cerr << "Usage: " << programName << " board_count [--all-neighbors]" << endl;
}

int main(int argc, char** argv) {
    /*if (argc < 2 || argc > 3) {
        printUsageError(argv[0]);
        return 1;
    }

    if (argc == 3) {
        cout << argv[2] << endl;
        if (strcmp(argv[2], "--all-neighbors") != 0) {
            printUsageError(argv[0]);
            return 1;
        }

        g_allNeighbors = true;
    }*/

    //auto iterations = atol(argv[1]);
    auto iterations = 10000;
    auto start_time = chrono::system_clock::now();

    srand(time(NULL));

    SudokuBoard board{};
    stringstream output;

    for(auto i = 0; i < iterations; ++i){
        board.fillCells();
        output << board.serialize() << endl;
        if (g_allNeighbors) {
            board.reset();
        }
    }

    auto now = chrono::system_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
    cout << "time ms: "  << duration << '\n';
    cout << "Iterations "  << iterations << '\n';
    cout << "Last board: " << board.serialize() << '\n';
    cout << "boards per second " <<  (double) 1000 * iterations / duration << '\n';

    return 0;
}

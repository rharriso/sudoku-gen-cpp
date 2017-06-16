#include <iostream>
#include <vector>
#include <set>
#include <deque>
#include <sstream>
#include <thread>
#include <random>

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
    set<coord> neighbors{}; // neighbors to look at when filling from top-left to bottom right
    int value = 0;

    SudokuCell() {}

    void setPosition(coord pos) {
        this->pos = pos;

        // generate neighbors
        (g_allNeighbors) ? generateAllNeighbors() :
                           generateOptimalNeighbors();
    }

private:
    void generateAllNeighbors () {
        // generate row  & col neighbors
        for (int n = 0; n < SIZE; ++n) {
            neighbors.insert({n, pos.j});
            neighbors.insert({pos.i, n});
        }

        auto iFloor = (pos.i / THIRD) * THIRD;
        auto jFloor = (pos.j / THIRD) * THIRD;

        // generate cell neighbors
        for (int n = iFloor; n < iFloor + THIRD ; ++n) {
            for (int m = jFloor; m < jFloor + THIRD; ++m) {
                neighbors.insert({n, m});
            }
        }
    }

    void generateOptimalNeighbors () {
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

        // generate cell neighbors
        for (int i = iFloor; i <= pos.i ; ++i) {
            for (int j = jFloor; (i < pos.i && j < jFloor + THIRD) || j < pos.j ; ++j) {
                neighbors.insert({i, j});
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
            cout << "Unable to fill board" << '\n';
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
        cout << '\n' << "----------------------------------" << '\n';

        for(int i = 0; i < SIZE; i++) {
            cout << "| ";

            for(int j = 0; j < SIZE; j++) {
                auto index = resolvePosition({i, j}); // YOOOO structs
                auto cell = cells[index];
                auto value = cell->value;

                if (cell->value > 0)  {
                    cout << cell->value << "  ";
                } else {
                    cout << "   ";
                }

                if (j % THIRD == THIRD - 1) {
                    cout << "| ";
                }
            }
            if (i % THIRD == THIRD - 1) {
                cout << '\n' << "----------------------------------";
            }
            cout << '\n';
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
    cerr << "Usage: " << programName << " board_count [--all-neighbors]" << '\n';
}

int main(int argc, char** argv) {
    /*if (argc < 2 || argc > 3) {
        printUsageError(argv[0]);
        return 1;
    }

    if (argc == 3) {
        cout << argv[2] << '\n';
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
        output << board.serialize() << '\n';
        if (g_allNeighbors) {
            board.reset();
        }
    }

    auto now = chrono::system_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(now - start_time).count();
    auto boards_per_s = 10e6 * ((double)iterations) / duration;
    cout << "time micros: "  << duration << '\n';
    cout << "Iterations "  << iterations << '\n';
    cout << "Last board: " << board.serialize() << '\n';
    cout << "boards per second " <<  boards_per_s << '\n';

    return 0;
}

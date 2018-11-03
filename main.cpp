#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>
#include <deque>
#include <sstream>
#include <thread>
#include <random>
#include <string.h>

constexpr int SIZE = 9;
constexpr int THIRD = SIZE / 3;
const std::set<int> VALID_VALUES = {1, 2, 3, 4, 5, 6, 7, 8, 9};
bool g_allNeighbors = false;
std::random_device rd;
std::mt19937 randomGen(rd());

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
    std::set<coord> neighbors{}; // neighbors to look at when filling from top-left to bottom right
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

        // find the block top left coordinate
        auto iFloor = (pos.i / THIRD) * THIRD;
        auto jFloor = (pos.j / THIRD) * THIRD;

        // generate block neighbors
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

        // generate block neighbors
        for (int i = iFloor; i <= pos.i ; ++i) {
            for (int j = jFloor; (i < pos.i && j < jFloor + THIRD) || j < pos.j ; ++j) {
                neighbors.insert({i, j});
            }
        }
    }
};

using cellQueue = std::deque<std::shared_ptr<SudokuCell>>;

class SudokuBoard {
    cellQueue cells;

public:

    SudokuBoard() {
        for(int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                auto cell = std::make_shared<SudokuCell>(SudokuCell());
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
        if(!doFillCells(0)) {
            std::cout << "Unable to fill board" << '\n';
        }
    }

    bool doFillCells(int index) {
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
    std::shared_ptr<SudokuCell> at(int index) {
        return this->cells.at(index);
    }

    /**
     *
     */
    std::shared_ptr<SudokuCell> at(coord position) {
        auto index = this->resolvePosition(position);
        return this->at(index);
    }

    /**
     * print
     */
    void printBoard() {
        std::cout << '\n' << "----------------------------------" << '\n';

        for(int i = 0; i < SIZE; i++) {
            std::cout << "| ";

            for(int j = 0; j < SIZE; j++) {
                auto index = resolvePosition({i, j}); // YOOOO structs
                auto cell = cells[index];
                auto value = cell->value;

                if (cell->value > 0)  {
                    std::cout << cell->value << "  ";
                } else {
                    std::cout << "   ";
                }

                if (j % THIRD == THIRD - 1) {
                    std::cout << "| ";
                }
            }
            if (i % THIRD == THIRD - 1) {
                std::cout << '\n' << "----------------------------------";
            }
            std::cout << '\n';
        }
    }

    /**
     * serialize
     */
    std::string serialize() {
        std::stringstream ostream;

        for(auto cell: cells) {
            ostream << cell->value;
        }

        return ostream.str();
    }
};

void printUsageError(char *programName) {
    std::cerr << "Usage: " << programName << " board_count [--all-neighbors]" << '\n';
}

int main(int argc, char** argv) {
    if (argc < 2 || argc > 3) {
        printUsageError(argv[0]);
        return 1;
    }

    if (argc == 3) {
        std::cout << argv[2] << '\n';
        if (strcmp(argv[2], "--all-neighbors") != 0) {
            printUsageError(argv[0]);
            return 1;
        }

        g_allNeighbors = true;
    }

    auto iterations = atol(argv[1]);
    auto start_time = std::chrono::system_clock::now();

    srand(time(NULL));

    SudokuBoard board{};
    std::stringstream output;

    for(auto i = 0; i < iterations; ++i){
        board.fillCells();
        output << board.serialize() << '\n';
        if (g_allNeighbors) {
            board.reset();
        }
    }

    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - start_time).count();
    auto boards_per_s = 10e5 * ((double)iterations) / duration;
    std::cout << "time micros: "  << duration << '\n';
    std::cout << "Iterations "  << iterations << '\n';
    std::cout << "Last board: " << board.serialize() << '\n';
    std::cout << "boards per second " <<  boards_per_s << '\n';

    return 0;
}

#include <iostream>
#include <vector>
#include <set>
#include <experimental/random>

using namespace std;

constexpr int SIZE = 9;
constexpr int THIRD = SIZE / 3;


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
    set<coord> neighbors{};
    coord position;
    int value = 0;

    SudokuCell() {}

    void setPosition(coord pos) {
        this->position = pos;

        // generate row neighbors
        for (int j = 0; j < SIZE; ++j) {
            neighbors.insert({pos.i, j});
        }

        // generate col neighbors
        for (int i = 0; i < SIZE; ++i) {
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


class SudokuBoard {
    vector<SudokuCell> cells = vector<SudokuCell>(SIZE*SIZE);

public:

    SudokuBoard() {
        for(int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                coord pos{i, j};
                auto index = resolvePosition(pos); // YOOOO structs
                cells[index].setPosition(pos);
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

    /**
     *
     */
    SudokuCell* at(int index) {
        return &(this->cells.at(index));
    }

    /**
     *
     */
    SudokuCell* at(coord position) {
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

                //cout << cell.value << "  ";
                cout << cell.value << "  ";

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
    SudokuBoard board{};

    auto cell = board.at({5, 3});
    cell->value = 1;

    for(auto neighbor : cell->neighbors){
        auto neighborCell = board.at(neighbor);
        neighborCell->value = 7;
    }

    board.printBoard();

    return 0;
}

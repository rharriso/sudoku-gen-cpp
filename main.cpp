#include <iostream>
#include <vector>
#include <experimental/random>

using namespace std;

constexpr int SIZE = 9;
constexpr int THIRD = SIZE / 3;


struct coord {
    int i = 0;
    int j = 0;
    inline bool operator ==(const coord &other) {
        return i == other.i && j == other.j;
    }
};


class SudokuCell {
public:
    coord position;
    int value = 0;

    SudokuCell() {}

    void setPosition(coord pos) {
        this->position = pos;

        // generate row neighbors
        // generate col neighbors
        // generate cell neighbors
        // get unique set of neighbors
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
    SudokuCell at(int index) {
        return this->cells[index];
    }

    /**
     *
     */
    SudokuCell at(coord position) {
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
                auto position = cell.position;

                //cout << cell.value << "  ";
                cout << position.i << "," << position.j << "  ";

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
    auto a = board.resolveIndex(80);
    auto b = board.resolveIndex(75);
    auto aa = board.resolvePosition(a);
    auto bb = board.resolvePosition(b);

    cout << a.i << ' ' << a.j << endl;
    cout << aa << endl;
    cout << b.i << ' ' << b.j << endl;
    cout << bb << endl;

    board.printBoard();

    return 0;
}

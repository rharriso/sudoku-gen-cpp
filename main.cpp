#include <iostream>
#include <vector>
#include <experimental/random>

using namespace std;

const int SIZE = 9;
const int THIRD = 3;

struct coord {
    int i = 0;
    int j = 0;
    inline bool operator ==(const coord &other) {
        return i == other.i && j == other.j;
    }
};

class SudokuCell {
    coord position;


public:
   SudokuCell() {}

   void setPosition(coord position) {
       this->position = position;
   }
};

class SudokuBoard {
    vector<SudokuCell> cells = vector<SudokuCell>(SIZE*SIZE);

public:

    SudokuBoard() {}

    /**
     * take index and return 2d coord
     */
    coord resolveIndex(int index) {
        return coord{
                index / SIZE,
                index % SIZE
        };
    }
};



void printBoard(const vector<vector<int>> &board) {
    cout << endl << "----------------------------------" << endl;

    for(int i = 0; i < SIZE; i++) {
        cout << "| ";

        for(int j = 0; j < SIZE; j++) {
            cout << board[i][j] << "  ";
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

int main() {
    SudokuBoard board{};
    auto a = board.resolveIndex(80);
    auto b = board.resolveIndex(75);

    cout << a.i << ' ' << a.j << endl;
    cout << b.i << ' ' << b.j << endl;

    return 0;
}

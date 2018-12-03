#include "sudoku-gen.h"

#include <iostream>
#include <thread>

void printUsageError(char *programName) {
    std::cerr << "Usage: " << programName << " board_count" << '\n';
}

int main(int argc, char** argv) {
   if (argc < 2 || argc > 3) {
        printUsageError(argv[0]);
        return 1;
    }

    //auto iterations = atol(argv[1]);
    auto iterations = 100; //atol(argv[1]);
    auto start_time = std::chrono::system_clock::now();
    std::stringstream output;

    srand(time(NULL));


    for(auto i = 0; i < iterations; ++i){
        auto board = generateAndFillBoard();
        output << board.serialize() << '\n';
    }

    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - start_time).count();
    auto boards_per_s = 10e5 * ((double)iterations) / duration;
    std::cout << "time micros: "  << duration << '\n';
    std::cout << "Iterations "  << iterations << '\n';
    std::cout << "boards per second " <<  boards_per_s << '\n';
    std::cout << output.str();

    return 0;
}

// Bruno Roy 2016-09-03
// Desc: Implementation of a Sudoku solver

#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <assert.h>

class Sudoku {
public:
    Sudoku() = delete;

    // std::string constructor
    Sudoku(const std::string& unsolvedSudoku) {
        // Creating the peers
        buildPeers();

        assert(unsolvedSudoku.size() == SUDOKU_SIZE);

        // Initialize all the squares with DIGITS
        for (auto i = 0; i < SUDOKU_SIZE; ++i) {
            values_.insert(std::make_pair(i, DIGITS));
        }
        // Assign a value to the squares that have an initial value
        for (auto i = 0; i < SUDOKU_SIZE; ++i) {
            if (unsolvedSudoku[i] != '0') {
                assign(i, unsolvedSudoku[i]);
            }
        }
    }

    // Builds the peers_ map
    void buildPeers() {
        for (auto j = 0; j < SUDOKU_SIZE; ++j) {
            std::unordered_set<int> peers;
            auto rowIndex = (j / 9);
            auto columnIndex = (j % 9);
            auto boxIndex = ((columnIndex / 3) * 3) + ((j / 27) * 27);
            for (auto i = 0; i < 9; ++i) {
                peers.insert(9 * rowIndex + i);                           // rows
                peers.insert(columnIndex + (9 * i));                      // columns
                peers.insert(boxIndex + (i % 3) + (9 * (i / 3))); // big 3x3 square
            }
            // Remove square from its peers
            peers.erase(j);
            peers_.insert(std::make_pair(j, peers));
        }
    }

    // Assign value to the square position, and propagate the values
    void assign(const int square, const char value) {
        values_[square] = value;
        eliminate(square, value);
    }

    // Eliminate value from the square peers
    void eliminate(const int square, const char value) {
        for (const auto& peer : peers_[square]) {
            std::string peerValue = values_[peer];
            // Remove the value if the peer contains it
            if (peerValue.find(value) != std::string::npos && peerValue.size() > 1) {
                peerValue.erase(std::remove(peerValue.begin(), peerValue.end(), value), peerValue.end());
                if (peerValue.size() == 1) {
                    assign(peer, peerValue[0]);
                } else {
                    values_[peer] = peerValue;
                }
            }
        }
    }

    // Print the Sudoku
    void print() const {
        int i = 0;
        for (const auto& pair : values_) {
            std::cout << pair.second << " | ";
            ++i;
            if (i % 9 == 0) {
                std::cout << std::endl;
            }
        }
    }

private:
    // Map square position (0 - 80) to std::string representing the possible values.
    std::map<int, std::string> values_;
    // Map square position (0 - 80) to a set of square position that are peers.
    std::unordered_map<int, std::unordered_set<int>> peers_;
    // Starting values of a square (all possible digits)
    const std::string DIGITS = "123456789";
    const int SUDOKU_SIZE = 81;
};

int main() {
    std::string unsolvedSudoku = "003020600900305001001806400008102900700000008006708200002609500800203009005010300";
    Sudoku sudoku(unsolvedSudoku);
    sudoku.print();
    return 0;
}
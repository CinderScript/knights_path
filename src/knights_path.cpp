#include <iostream>

#include <map>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <climits>
#include <algorithm>

using std::map;
using std::vector;
using std::queue;
using std::stack;
using std::set;

using std::max;
using std::min;

using std::cout;
using std::endl;
using std::pair;
using std::max;
using std::min_element;


class Chessboard {
private:

    // file, then rank
    map<char, vector<int>> board;
    vector<int> fileMoveSet = { 1,  1, -1, -1, 2,  2, -2, -2 };
    vector<int> rankMoveSet = { 2, -2,  2, -2, 1, -1,  1, -1 };
    const pair<char, int> INVALID = {'*', -123};

public:

    Chessboard() {
        for (char file = 'a'; file <= 'h'; file++) {
            board[file] = vector<int>(8, 0);
        }
    }

    void Clear(){
        for (char file = 'a'; file <= 'h'; file++) {
            board[file] = vector<int>(8, 0);
        }
    }

    void PrintBoard() {
        // Start at the top of the board -so rank 8
        // and print each lesser rank after that.
        for (int rank = 8; rank >= 1; rank--) {
            cout << rank << "| ";

            // print each column (file) of this rank
            for (char file = 'a'; file <= 'h'; file++) {
                auto square = board[file][rank - 1];
                if (square == 0)
                    cout << '.' << "   ";
                else
                    cout << square << "   ";
            }

            if ( rank > 1)
                cout << "\n |\n";
        }

        cout << "\n   _____________________________\n   ";
        for (char file = 'A'; file <= 'H'; file++) {
            cout << file << "   ";
        }
        cout << "\n\n" << endl;
    }

    /// @brief Plot the moveset for a knight from a given square
    /// @param fileFrom 
    /// @param rankFrom 
    void PlotMoveSet (char fileFrom, int rankFrom) {
        board[fileFrom][rankFrom - 1] = 1;
        for (size_t i = 0; i < 8; i++){
            auto valid = PlotMove(fileFrom, rankFrom, i, i+2);
        }
    }

    /// @brief Find shortest path information for each possible square using Dijkstra's algorithm
    void FindDistanceToSquares(pair<char, int> start) {


        map<char, vector<pair<char, int>>> previousMove;

        Dijkstra(start, previousMove);
    }

private:
    bool Dijkstra(pair<char, int> start, map<char, vector<pair<char, int>>>& previousMove) {
        // initialize moveCount with infinity for all squares
        for (char file = 'a'; file <= 'h'; file++) {
            board[file] = vector<int>(8, INT_MAX);
        }
        
        // initialize previousMove with INVALID for all squares
        for (char file = 'a'; file <= 'h'; file++) {
            previousMove[file] = vector<pair<char, int>>(8, INVALID);
        }

        set<pair<char, int>> visited;

        auto minElement = std::min_element(visited.begin(), visited.end());
    }

    pair<char, int> GetMove(pair<char, int> square, int move){
        int fDelta = fileMoveSet[move];
        auto rDelta = rankMoveSet[move];

        char fileTo = square.first + fDelta;
        auto rankTo = square.second + rDelta;

        // if file is out of bounds
        if (fileTo < 'a' || fileTo > 'h')
            return INVALID;
        // if rank is out of bounds
        if (rankTo < 1 || rankTo > 8)
            return INVALID;

        return pair<char, int>(fileTo, rankTo);
    }
    bool PlotMove(char fileFrom, int rankFrom, int move, int value) {
        
        int fDelta = fileMoveSet[move];
        auto rDelta = rankMoveSet[move];

        char fileTo = fileFrom + fDelta;
        auto rankTo = rankFrom + rDelta;

        // if file is out of bounds
        if (fileTo < 'a' || fileTo > 'h')
            return false;
        // if rank is out of bounds
        if (rankTo < 1 || rankTo > 8)
            return false;


        board[fileTo][rankTo - 1] = value;
        return true;
    }
};


int main() {

    
    Chessboard chessboard;

    cout << "Possible moves from b2 & g7:\n";
    chessboard.PlotMoveSet('b', 2);
    chessboard.PlotMoveSet('g', 7);
    chessboard.PrintBoard();

    chessboard.Clear();


    return 0;
}

#include <iostream>

#include <map>
#include <vector>
#include <queue>
#include <list>
#include <set>
#include <climits>
#include <algorithm>

using std::map;
using std::vector;
using std::queue;
using std::list;
using std::set;

using std::max;
using std::min;

using std::cout;
using std::endl;
using std::pair;
using std::max;
using std::min_element;


struct FunFacts{
    vector<pair<char, int>> shortestPath;
    size_t squaresVisited = 0;
    size_t squareLookups = 0;
    size_t pathMoves = 0;
};

class Chessboard {
private:

    // file, then rank
    map<char, vector<int>> board;
    vector<int> fileMoveSet = { 1,  1, -1, -1, 2,  2, -2, -2 };
    vector<int> rankMoveSet = { 2, -2,  2, -2, 1, -1,  1, -1 };
    const pair<char, int> INVALID = {'*', -999};

    void PlotPath(vector<pair<char, int>> path) {
        for (size_t i = 0; i < path.size(); i++) {
            MarkSquare(path[i], i + 1);
        }
    }
    pair<char, int> GetSquare(pair<char, int> square, int move) {
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
    void MarkSquare(pair<char, int> square, int value) {
        board[square.first][square.second - 1] = value;
    }

    bool PlotMove(pair<char, int> startSquare, int move, int value) {

        auto nextSquare = GetSquare(startSquare, move);
        if (nextSquare != INVALID) {
            MarkSquare(nextSquare, value);
            return true;
        }
        return false;
    }

public:

    Chessboard() {
        Clear();
    }

    void Clear(){
        for (char file = 'a'; file <= 'h'; file++) {
            board[file] = vector<int>(8, -999);
        }
    }

    void PrintBoard(bool showMoveset = false) {
        // Start at the top of the board -so rank 8
        // and print each lesser rank after that.
        for (int rank = 8; rank >= 1; rank--) {
            cout << rank << "| ";
            // print each column (file) of this rank
            for (char file = 'a'; file <= 'h'; file++) {
                auto square = board[file][rank - 1];
                if (square == -999)
                    cout << ' ' << "   ";
                else if (square == -1)
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
        board[fileFrom][rankFrom - 1] = -1;
        for (size_t i = 0; i < 8; i++){
            auto valid = PlotMove({fileFrom, rankFrom}, i, i+1);
        }
    }

    FunFacts PlotShortestPath(pair<char, int> start, pair<char, int> target) {
        FunFacts funfacts;
        queue<pair<char, int>> potential;
        map<pair<char, int>, pair<char, int>> history;
        potential.push(start);
        MarkSquare(start, -1);  // mark as visited

        while (!potential.empty()) {

            // process the next potential square
            auto current = potential.front();

            potential.pop();

            // found it
            if (current == target) {

                while (current != start) {
                    funfacts.shortestPath.push_back(current);
                    current = history[current]; //backtrack
                }
                funfacts.shortestPath.push_back(start);

                std::reverse(
                    funfacts.shortestPath.begin(), 
                    funfacts.shortestPath.end());

                PlotPath(funfacts.shortestPath);

                funfacts.pathMoves = funfacts.shortestPath.size() - 1;
                return funfacts;
            }

            // for each possible move from this square
            for (size_t i = 0; i < 8; i++) {
                auto nextSquare = GetSquare(current, i);
                funfacts.squareLookups++;

                if (nextSquare != INVALID) {
                    if (board[nextSquare.first][nextSquare.second - 1] != -1) {
                        potential.push(nextSquare);
                        MarkSquare(nextSquare, -1); // mark as visited
                        history[nextSquare] = current;
                        funfacts.squaresVisited++;
                    }
                }
            }
        }

        // no path found
        return funfacts;
    }


};


int main() {
    Chessboard chessboard;

    cout << "Possible moves from b2 & g7:\n";
    chessboard.PlotMoveSet('b', 2);
    chessboard.PlotMoveSet('g', 7);
    chessboard.PrintBoard();
    chessboard.Clear();

    cout << "Running BFS on b2 to g8:\n";
    
    auto path = chessboard.PlotShortestPath({'b', 2}, {'g', 8});
    chessboard.PrintBoard();

    cout << "------------ Fun Facts ------------\n";
    cout << "Path length:\t\t" << path.pathMoves << "\n";
    cout << "Total steps:\t\t" << path.squaresVisited << "\n";
    cout << "Total Square Lookups:\t" << path.squareLookups << "\n";
    cout << "Path Found -\n";

    for (size_t i = 0; i < path.shortestPath.size(); i++) {
        cout << path.shortestPath[i].first << path.shortestPath[i].second;
        if (i < path.shortestPath.size() - 1)
            cout << " -> ";
        if ((i + 1) % 6 == 0) 
            cout << "\n"; 
    }


    cout << "\n\n" << endl;
    return 0;
}

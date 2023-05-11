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


struct FunFacts {
    vector<pair<char, int>> shortestPath;
    size_t squaresVisited = 0;
    size_t squareLookups = 0;
    size_t pathMoves = 0;
    bool found = false;
};

class Chessboard {
private:

    // file, then rank
    map<char, vector<int>> board;
    vector<int> fileMoveSet = { 1,  1, -1, -1, 2,  2, -2, -2 };
    vector<int> rankMoveSet = { 2, -2,  2, -2, 1, -1,  1, -1 };
    const pair<char, int> INVALID = { '*', -999 };

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

    void Clear() {
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
            if (rank > 1)
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
    void PlotMoveSet(char fileFrom, int rankFrom) {
        board[fileFrom][rankFrom - 1] = -1;
        for (size_t i = 0; i < 8; i++) {
            auto valid = PlotMove({ fileFrom, rankFrom }, i, i + 1);
        }
    }

    FunFacts PlotShortestPath(pair<char, int> start, pair<char, int> target) {
        Clear();
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
                funfacts.found = true;
                return funfacts;
            }

            // for each possible move from this square
            for (size_t i = 0; i < 8; i++) {
                auto nextSquare = GetSquare(current, i);

                if (nextSquare != INVALID) {
                    funfacts.squareLookups++;
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

    vector<pair<char, int>> GetAllSquares() {
        vector<pair<char, int>> squares;
        for (char file = 'a'; file <= 'h'; file++) {
            for (int rank = 1; rank <= 8; rank++) {
                squares.push_back({ file, rank });
            }
        }
        return squares;
    }
};


/* * * SOME EXPERIMENTS * * */
size_t CheckEveryPathFrom(Chessboard& board,
    pair<char, int> start,
    size_t& squaresWithPath,
    size_t& squaresWithNoPath,
    size_t& longestPath,
    size_t& pathsWith1Moves,
    size_t& pathsWith2Moves,
    size_t& pathsWith3Moves,
    size_t& pathsWith4Moves,
    size_t& pathsWith5Moves,
    size_t& pathsWith6Moves,
    FunFacts& totals) {
    squaresWithPath = 0;
    squaresWithNoPath = 0;
    longestPath = 0;
    pathsWith1Moves = 0;
    pathsWith2Moves = 0;
    pathsWith3Moves = 0;
    pathsWith4Moves = 0;
    pathsWith5Moves = 0;
    pathsWith6Moves = 0;

    auto allSquares = board.GetAllSquares();

    //vector<pair<char, int>> squaresWithNoPath;
    size_t count = 0;
    // for every square on the board
    for (size_t i = 0; i < allSquares.size(); i++) {
        if (start == allSquares[i])
            continue;

        // Find the shortest path from a1 to every other square
        auto fun = board.PlotShortestPath(start, allSquares[i]);
        count++;

        // totals
        totals.squareLookups += fun.squareLookups;
        totals.squaresVisited += fun.squaresVisited;
        totals.pathMoves += fun.pathMoves;

        if (fun.pathMoves == 6) {
            pathsWith6Moves++;
        }
        else if (fun.pathMoves == 5) {
            pathsWith5Moves++;
        }
        else if (fun.pathMoves == 4) {
            pathsWith4Moves++;
        }
        else if (fun.pathMoves == 3) {
            pathsWith3Moves++;
        }
        else if (fun.pathMoves == 2) {
            pathsWith2Moves++;
        }
        else if (fun.pathMoves == 1) {
            pathsWith1Moves++;
        }

        // longest path
        if (fun.pathMoves > longestPath) {
            longestPath = fun.pathMoves;
        }

        // no path?
        if (!fun.found) {
            squaresWithNoPath++;
        }
        else {
            squaresWithPath++;
        }
    }

    return count;
}


int main() {
    Chessboard chessboard;

    cout << "Possible moves from b2 & g7:\n";
    chessboard.PlotMoveSet('b', 2);
    chessboard.PlotMoveSet('g', 7);
    chessboard.PrintBoard();
    chessboard.Clear();

    cout << "Running BFS on b2 to g8:\n";

    auto path = chessboard.PlotShortestPath({ 'b', 2 }, { 'g', 8 });
    chessboard.PrintBoard();

    cout << "------------ Fun Facts ------------\n";
    cout << "Path length:\t\t" << path.pathMoves << "\n";
    cout << "Squares Visited:\t\t" << path.squaresVisited << "\n";
    cout << "Total Square Lookups:\t" << path.squareLookups << "\n";
    cout << "Path Found -\n";
    for (size_t i = 0; i < path.shortestPath.size(); i++) {
        cout << path.shortestPath[i].first << path.shortestPath[i].second;
        if (i < path.shortestPath.size() - 1)
            cout << " -> ";
        if ((i + 1) % 6 == 0)
            cout << "\n";
    }


    /* * * SOME EXPERIMENTS * * */
    auto allSquares = chessboard.GetAllSquares();

    FunFacts totals;
    size_t squaresWithPath = 0;
    size_t squaresWithNoPath = 0;
    size_t longestPath = 0;
    size_t pathsWith1Moves = 0;
    size_t pathsWith2Moves = 0;
    size_t pathsWith3Moves = 0;
    size_t pathsWith4Moves = 0;
    size_t pathsWith5Moves = 0;
    size_t pathsWith6Moves = 0;



    pair<char, int> start = { 'a', 1 };
    size_t count = CheckEveryPathFrom(
        chessboard, start,
        squaresWithPath,
        squaresWithNoPath,
        longestPath,
        pathsWith1Moves,
        pathsWith2Moves,
        pathsWith3Moves,
        pathsWith4Moves,
        pathsWith5Moves,
        pathsWith6Moves,
        totals);

    cout << "\n\nChecking every path starting from: " << start.first << start.second << ":\n";
    cout << "Squares checked:\t" << count << "\n";
    cout << "Squares with a path:\t" << squaresWithPath << "\n";
    cout << "Squares with no path:\t" << squaresWithNoPath << "\n";
    cout << "Longest shortest path:\t" << longestPath << "\n";
    cout << "Paths with 1 move:\t" << pathsWith1Moves << "\n";
    cout << "Paths with 2 moves:\t" << pathsWith2Moves << "\n";
    cout << "Paths with 3 moves:\t" << pathsWith3Moves << "\n";
    cout << "Paths with 4 moves:\t" << pathsWith4Moves << "\n";
    cout << "Paths with 5 moves:\t" << pathsWith5Moves << "\n";
    cout << "Paths with 6 moves:\t" << pathsWith6Moves << "\n";
    cout << "Total Square Lookups:\t" << totals.squareLookups << "\n";
    cout << "Total Squares Visited:\t" << totals.squaresVisited << "\n";

    cout << "\n\nIf a knight can reach any square in 6 moves from a1, then the knight can\n"
            "reach any square starting from any square in at least 6 moves.\n\n";

    cout << endl;
    return 0;
}

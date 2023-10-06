#include "Board.h"
#include <iostream>
#include <vector>
#include <string>

//goal state
int goal[4][4] = { {1, 2, 3, 4}, 
                   {5, 6, 7, 8},
                   {9, 10, 11, 12},
                   {13, 14, 15, 0}
                 };

//constructor (with operator specified)
Board::Board(int s[4][4], Board *p, char o) {
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            state[r][c] = s[r][c];
    parent = p;
    op = o;

    set_heuristic();
    set_g();
    set_f();
}

//constructor (from user input)
Board::Board(std::istream& i) {
    std::string line;

    int row = 0;

    std::cout << "Enter the board: " << std::endl;

    while (std::getline (i, line), !line.empty()) {
        //parse numbers from string and put them into board using stoi
        for (int i = 0; i < 4; i++) {
            state[row][i] = std::stoi(line.substr(0, line.find(" ")));
            line.erase(0, line.find(" ") + 1);
        }
        row++;
    }

    parent = NULL;
    op = 'I'; //mark initial state for operator

    set_heuristic();
    set_g();
    set_f();
}

//default constructor
Board::Board() {
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            state[r][c] = 0;
    parent = NULL;

    set_heuristic();
    set_g();
    set_f();
}

//copy constructor
Board::Board(Board *b) {
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            state[r][c] = b->state[r][c];
    parent = b->parent;
    op = b->op;
    h = b->h;
    g = b->g;
    f = b->f;
}

//destructor
Board::~Board() {
    parent = NULL;
}

//prints the board
void Board::printBoard() const {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (state[r][c] == 0) //make 0 colored red
                std::cout << "\033[1;31m" << " X" << "\033[0m" << " ";
            else {
                if (state[r][c] < 10)
                    std::cout << " "; //add space before single digit numbers to make board look nicer
                std::cout << state[r][c] << " ";
            }
        }
        std::cout << std::endl;
    }
}

//prints the path from the initial state to the current state using operators
void Board::printOperatorPath () {
    std::vector<Board> path; 
    Board current = *this;

    // backtrack from the goal state to the initial state
    while (current.op != 'I') {
        path.push_back(current);
        current = current.getParent();
    }
    path.push_back(current);

    // print the path in reverse order
    for (auto it = path.rbegin(); it != path.rend(); ++it) {        
        switch (it->get_operator()) {
            case 'U':
                std::cout << "UP";
                break;
            case 'D':
                std::cout << "DOWN";
                break;
            case 'L':
                std::cout << "LEFT";
                break;
            case 'R':
                std::cout << "RIGHT";
                break;
            default:
                std::cout << "INITIAL";
                break;
        }
        //print down arrow (except for last operator)
        if (it != path.rend() - 1)
            std::cout << " -> ";
    }
    std::cout << std::endl;
}

//prints the path from the initial state to the current state using states (with initial state first, then solution)
void Board::printStatePath() {
    std::vector<Board> path;
    Board current = *this;

    // backtrack from the goal state to the initial state
    while (current.op != 'I') {
        path.push_back(current);
        current = current.getParent();
    }
    path.push_back(current);

    // print the path in reverse order
    for (auto it = path.rbegin(); it != path.rend(); ++it) {
        it->printBoard();

        //print down arrow (except for last iteration)
        if (it != path.rend() - 1)
            std::cout << "     ↓" << std::endl;
    }
}

//checks if the board is the goal state
bool Board::isGoal() {
    if (state[3][3] != 0)
        return false; //blank in wrong spot, not goal

    //iterate thru board, check if all tiles in correct position
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            if (state[r][c] != goal[r][c])
                return false; //misplaced tile, not goal
    
    return true; //no misplaced tiles, goal
}

void Board::set_g() {
    if (parent == NULL)
        g = 0;
    else
        g = parent->g + 1;
}

void Board::set_f() {
    f = g + h;
}

//heuristic functions

void Board::set_heuristic() { h = h1() + h2() + h3() + h4(); }

//returns the heuristic value of the board
int Board::get_heuristic() { return h; }

//1st heuristic: number of misplaced tiles
int Board::h1() const {
    int misplaced = 0;

    for ( int r = 0; r < 4; r++ )
        for ( int c = 0; c < 4; c++ )
            if ( state[r][c] != goal[r][c] )
                misplaced++;

    return misplaced;
}

//2nd heuristic: manhattan distance
int Board::h2() const {
    int distance = 0;

    for (int r=0; r<4; r++) {
        for (int c=0; c<4; c++) {
            int tile = state[r][c];

            if (tile != 0) {
                int goal_r, goal_c;

                //find goal position of tile
                for (int i=0; i<4; i++) {
                    for (int j=0; j<4; j++) {
                        if (goal[i][j] == tile) {
                            goal_r = i;
                            goal_c = j;
                        }
                    }
                }

                //add distance of tile from goal position to total distance
                distance += abs(r - goal_r) + abs(c - goal_c);
            }
        }
    }

    return distance;
}

//3rd heuristic: linear conflict
int Board::h3() const {
    int conflict = 0;

    //check rows
    for (int r = 0; r < 4; r++) {
        int max = -1;
        for (int c = 0; c < 4; c++) {
            int tile = state[r][c];
            if (tile != 0 && tile / 4 == r) {
                if (tile > max)
                    max = tile;
                else
                    conflict += 2;
            }
        }
    }

    //check columns
    for (int c = 0; c < 4; c++) {
        int max = -1;
        for (int r = 0; r < 4; r++) {
            int tile = state[r][c];
            if (tile != 0 && tile % 4 == c) {
                if (tile > max)
                    max = tile;
                else
                    conflict += 2;
            }
        }
    }

    return conflict;
}

//4th heuristic: inversion count
int Board::h4() const {
    int inversionCount = 0;
    
    // Flatten the 2D board into a 1D array
    int flattened[16];

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            flattened[i * 4 + j] = state[i][j];

    for (int i = 0; i < 16; ++i) {
        if (flattened[i] == 0) // Assuming the blank tile is represented as 0
            continue;
        for (int j = i + 1; j < 16; ++j) {
            if (flattened[j] == 0)
                continue;
            if (flattened[i] > flattened[j])
                inversionCount++;
        }
    }
    
    return inversionCount;
}

//finds the coordinates blank space in the board, represented by variables r and c
void Board::find_blank(int &r, int &c) {
    for (r = 0; r < 4; r++)
        for (c = 0; c < 4; c++)
            if (state[r][c] == 0)
                return;
}

char Board::get_operator() { return op; }

int Board::get_g() { return g; }

int Board::get_f() { return f; }

//operators

//move blank space up
Board Board::moveUp() {
    //find blank space coordinates (r, c)
    int r, c;
    find_blank(r, c);

    //check if blank space can move up
    if (r == 0)
        return *this; //blank space cannot move up, return current board object

    std::swap(state[r][c], state[r - 1][c]); //swap blank space with tile above it

    Board result (state, this, 'U'); //return new board with swapped tiles

    std::swap(state[r][c], state[r - 1][c]); //swap tiles back to restore original state

    return result; //return new board with swapped tiles
}

//move blank space down
Board Board::moveDown() {
    //find blank space coordinates (r, c)
    int r, c;
    find_blank(r, c);

    //check if blank space can move down
    if (r == 3)
        return *this; //blank space cannot move down, return current board object

    std::swap(state[r][c], state[r + 1][c]); //swap blank space with tile below it

    Board result (state, this, 'D'); //return new board with swapped tiles

    std::swap(state[r][c], state[r + 1][c]); //swap tiles back to restore original state

    return result; //return new board with swapped tiles
}

Board Board::moveLeft() {
    int r, c;
    find_blank(r, c);

    //check if blank space can move left
    if (c == 0)
        return *this; //blank space cannot move left, return current board object

    std::swap(state[r][c], state[r][c - 1]); //swap blank space with tile to the left of it

    Board result (state, this, 'L'); //return new board with swapped tiles

    std::swap(state[r][c], state[r][c - 1]); //swap tiles back to restore original state

    return result; //return new board with swapped tiles
}

//move blank space right
Board Board::moveRight() {
    int r, c;
    find_blank(r, c);

    //check if blank space can move right
    if (c == 3)
        return *this; //blank space cannot move right, return current board object

    std::swap(state[r][c], state[r][c + 1]); //swap blank space with tile to the right of it

    Board result (state, this, 'R'); //return new board with swapped tiles

    std::swap(state[r][c], state[r][c + 1]); //swap tiles back to restore original state

    return result; //return new board with swapped tiles
}

//returns the parent of the board
Board* Board::getParent() { return parent; }

//overloaded operators for Board class
bool Board::operator== (const Board &b) const {
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            if (state[r][c] != b.state[r][c])
                return false;
    return true;
}

bool Board::operator!=(const Board &b)  const { return !(*this == b); }

bool operator<(const Board &b1, const Board &b2) { return b1.f > b2.f; }
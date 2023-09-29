#include "Board.h"
#include <iostream>
#include <string>

//constructor
Board::Board(int s[4][4], Board *p) {
    for (int r = 0; r < 4; r++)
    for (int c = 0; c < 4; c++)
        state[r][c] = s[r][c];
    parent = p;
    open = true; //set node open
}

//constructor
Board::Board(int s[4][4], Board *p, bool o) {
    for (int r = 0; r < 4; r++)
    for (int c = 0; c < 4; c++)
        state[r][c] = s[r][c];
    parent = p;
    open = o; //set node open
}

//constructor (from user input)
Board::Board(std::istream& i) {
    open = true; //set node open

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
}

//default constructor
Board::Board() {
    open = true; //set node open

    for (int r = 0; r < 4; r++)
    for (int c = 0; c < 4; c++)
        state[r][c] = 0;
    parent = NULL;
}

//copy constructor
Board::Board(Board *b) {
    for (int r = 0; r < 4; r++)
    for (int c = 0; c < 4; c++)
        state[r][c] = b->state[r][c];
    parent = b->parent;
}

//destructor
Board::~Board() {
    parent = NULL;
}

//prints the board
void Board::printBoard() {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++)
            std::cout << state[r][c] << " ";
        std::cout << std::endl;
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

bool Board::isOpen() {
    return open;
}

//returns the heuristic value of the board
int Board::heuristic() {
    return h1() + h2();
}

//number of misplaced tiles
int Board::h1() {
    int misplaced = 0;

    for ( int r = 0; r < 4; r++ )
        for ( int c = 0; c < 4; c++ )
            if ( state[r][c] != goal[r][c] )
                misplaced++;

    return misplaced;
}

//sum of the distances of the tiles from their goal positions
int Board::h2() {
    int distance = 0;

    for ( int r = 0; r < 4; r++ )
        for ( int c = 0; c < 4; c++ )
            for ( int i = 0; i < 4; i++ )
                for ( int j = 0; j < 4; j++ )
                    if ( state[r][c] == goal[i][j] )
                        distance += abs(r - i) + abs(c - j);

    return distance;
}

//finds the coordinates blank space in the board, represented by variables r and c
void Board::find_blank (int &r, int &c) {
    for (r = 0; r < 4; r++)
        for (c = 0; c < 4; c++)
            if (state[r][c] == 0)
                break;
}

//operators

//move tile up into blank space above it
Board Board::moveUp() {
    //find blank space
    int r, c;
    find_blank(r, c);

    //if blank space is in top row, return same board (no move)
    if (r == 0)
        return *this;

    //declare temp array and copy state into it
    int temp[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            temp[i][j] = state[i][j];

    std::swap (temp[r][c], temp[r-1][c]);

    return Board(temp, this);
}

//move tile down into blank space below it
Board Board::moveDown() {
    int r, c;
    find_blank(r, c);

    if (r == 3) //if blank space is in bottom row, return same board (no move)
        return *this;

    int temp[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) 
            temp[i][j] = state[i][j];
        
    std::swap (temp[r][c], temp[r+1][c]);

    return Board(temp, this);
}

//move tile left into blank space to the left of it
Board Board::moveLeft() {
    int r, c;
    find_blank(r, c);

    if (c == 0) //if blank space is in leftmost column, return same board (no move)
        return *this;

    int temp[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) 
            temp[i][j] = state[i][j];
        
    std::swap (temp[r][c], temp[r][c-1]);

    return Board(temp, this);
}

//move tile right into blank space to the right of it
Board Board::moveRight() {
    int r, c;
    find_blank(r, c);

    if (c == 3) //if blank space is in rightmost column, return same board (no move)
        return *this;

    int temp[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) 
            temp[i][j] = state[i][j];

    std::swap (temp[r][c], temp[r][c+1]);

    return Board(temp, this);
}

//returns the parent of the board
Board* Board::getParent() {
    return parent;
}

//overloaded == operator
bool Board::operator== (const Board &b) {
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            if (state[r][c] != b.state[r][c])
                return false;
    return true;
}

bool Board::operator!=(const Board &b) {
    return !(*this == b);

    /* for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            if (state[r][c] != b.state[r][c])
                return true;
        return false;
    */
}

//returns the state of the board
/* int** Board::getState() {
    return state;
} */
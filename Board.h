#include <iostream>

class Board {
    int goal[4][4] = {{1, 2, 3, 4},
                {5, 6, 7, 8},
                {9, 10, 11, 12},
                {13, 14, 15, 0}};

    public:
        Board(std::istream& in); //constructor
        Board(int s[4][4], Board *p); //constructor
        Board(int s[4][4], Board *p, bool o); //constructor
        Board(); //default constructor
        Board(Board *b); //copy constructor
        ~Board(); //destructor
        void printBoard(); //prints the board
        bool isGoal(); //checks if the board is the goal state
        int heuristic(); //returns the heuristic value of the board

        //getter functions
        Board* getParent(); //returns the parent node
        bool isOpen(); //returns if the node is open

        //operators
        Board moveUp();
        Board moveDown();
        Board moveLeft();
        Board moveRight();

        //overloaded operators
        bool operator==(const Board &b) const;
        bool operator!=(const Board &b) const;
    private:
        bool open; //checks if the board (node) is open
        int h1(); //number of misplaced tiles
        int h2(); //sum of the distances of the tiles from their goal positions
        int state[4][4]; //the board
        Board *parent; //parent node
        void find_blank(int &r, int &c); //finds coordinates of the blank tile
};
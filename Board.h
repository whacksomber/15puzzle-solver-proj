#include <iostream>

class Board {
    public:
        //constructors (and destructor)
        Board(std::istream& in);               //constructor
        Board (int s[4][4], Board *p, char o); //constructor (with operator)
        Board();                               //default constructor
        Board(Board *b);                       //copy constructor
        ~Board();                              //destructor
        
        void printBoard() const;   //prints the board
        bool isGoal();             //checks if the board is the goal state
        void printOperatorPath (); //prints the path from the initial state to the current state using operators
        void printStatePath ();    //prints the path from the initial state to the current state using states

        //getter functions
        Board* getParent();  //returns the parent node
        int get_heuristic(); //returns the heuristic value of the board
        char get_operator(); //returns operator used to achieve current state (U, D, L, R)
        int get_g();         //returns the cost of the path from the initial state to the current state

        //operators
        Board moveUp();    //move blank space up
        Board moveDown();  //move blank space down
        Board moveLeft();  //move blank space left
        Board moveRight(); //move blank space right

        //overloaded operators
        bool operator==(const Board &b) const; //overloaded == operator
        bool operator!=(const Board &b) const; //overloaded != operator

        friend bool operator<(const Board &b1, const Board &b2); //overloaded < operator

    private:
        //heuristic functions
        void set_g();
        void set_heuristic(); //sets the heuristic value (member variable h) of the board
        int h1() const;       //1st heuristic: number of misplaced tiles
        int h2() const;       //2nd heuristic: sum of the distances of the tiles from their goal positions
        int h3() const;       //3rd heuristic: linear conflict
        int h;                //heuristic value
        int g;                //cost of the path from the initial state to the current state

        int state[4][4]; //the board
        Board *parent;   //parent node
        char op; //operator used to achieve current state ('U', 'D', 'L', 'R') ('I' for initial state)
        
        void find_blank(int &r, int &c); //finds coordinates of the blank tile
};

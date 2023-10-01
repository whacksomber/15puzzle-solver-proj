#include <iostream>

class Board {
    //goal state
    int goal[4][4] = { {1, 2, 3, 4},
                       {5, 6, 7, 8},
                       {9, 10, 11, 12},
                       {13, 14, 15, 0}
                     };

    public:
        //constructors (and destructor)
        Board(std::istream& in);               //constructor
        Board(int s[4][4]);                    //constructor (for debugging, delete later)
        Board(int s[4][4], Board *p);          //constructor
        Board (int s[4][4], Board *p, char o); //constructor (with operator)
        Board();                               //default constructor
        Board(Board *b);                       //copy constructor
        ~Board();                              //destructor
        
        void printBoard() const; //prints the board
        bool isGoal();     //checks if the board is the goal state
        
        void printOperatorPath (); //prints the path from the initial state to the current state using operators
        void printStatePath ();    //prints the path from the initial state to the current state using states

        //getter functions
        Board* getParent();          //returns the parent node
        int get_heuristic() const;   //returns the heuristic value of the board
        int g() const;               //returns the length of the path from the initial state to the current state
        char get_operator();         //get operator used to achieve current state (U, D, L, R)
        int f() const;               //returns the sum of the heuristic value and the length of the path from the initial state to the current state
        
        //operators
        Board moveUp();    //move blank space up
        Board moveDown();  //move blank space down
        Board moveLeft();  //move blank space left
        Board moveRight(); //move blank space right

        //overloaded operators
        bool operator==(const Board &b) const; //overloaded == operator
        bool operator!=(const Board &b) const; //overloaded != operator
        void operator=(const Board &b);        //overloaded = operator

        friend bool operator<(const Board &b1, const Board &b2); //overloaded < operator

    private:
        //heuristic functions
        int h1() const;  //number of misplaced tiles
        int h2() const;  //sum of the distances of the tiles from their goal positions
        int h3() const;

        int state[4][4]; //the board
        Board *parent;   //parent node
        char op; //operator used to achieve current state ('U', 'D', 'L', 'R') ('I' for initial state)
        void find_blank(int &r, int &c); //finds coordinates of the blank tile
};

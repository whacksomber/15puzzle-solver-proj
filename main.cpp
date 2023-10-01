#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include "Board.h" //include Board class

//TODO: make Board member function to print path, and one to return the depth (# of moves) of the path
//TODO: make goal global or private

/* best-first search algorithm, returns the solution node
 * is = initital state
 * num = number of nodes generated
 */
Board best_first_search(Board is, int &num) {
    Board n (is); //create board/node object n, set to initial state on first iteration
    num = 1;      //set number of nodes generated to 1 (for initial state)

    //create two sets of boards, depending on whether nodes are yet to be visited (open) or have already been visited (closed)
    std::priority_queue<Board> open; //priority queue of boards, sorted by heuristic value
    std::vector<Board> open_vec;     //vector of open nodes, to keep track of duplicates
    std::vector<Board> closed;       //vector of visited boards

    open.push(n); //push initial state onto priority queue
    open_vec.push_back(n); //push initial state onto vector

    while (!n.isGoal()) { //while goal has not yet been found
        n.printBoard(); //print current state
        std::cout << std::endl;
        
        if (open.empty()) { //if open is empty
            std::cout << "No solution found." << std::endl;
            return Board(); //return empty Board object
        }

        //remove n from open and add it to closed
        open.pop();
        open_vec.erase(std::remove(open_vec.begin(), open_vec.end(), n), open_vec.end());
        closed.push_back(n);

        //expand n, get children
        Board up = n.moveUp();
        Board down = n.moveDown();
        Board left = n.moveLeft();
        Board right = n.moveRight();

        //add children to open if they are not already in open or closed and are results of legal moves
        if (std::find(closed.begin(), closed.end(), up) == closed.end() && up != n
            && std::find(open_vec.begin(), open_vec.end(), up) == open_vec.end() ) {
            open.push(up);
            open_vec.push_back(up);
            num++;
        }
        if (std::find(closed.begin(), closed.end(), down) == closed.end() && down != n
            && std::find(open_vec.begin(), open_vec.end(), down) == open_vec.end()) {
            open.push(down);
            open_vec.push_back(down);
            num++;
        }
        if (std::find(closed.begin(), closed.end(), left) == closed.end() && left != n
            && std::find(open_vec.begin(), open_vec.end(), left) == open_vec.end()) {
            open.push(left);
            open_vec.push_back(left);
            num++;
        }
        if (std::find(closed.begin(), closed.end(), right) == closed.end() && right != n
            && std::find(open_vec.begin(), open_vec.end(), right) == open_vec.end()) {
            open.push(right);
            open_vec.push_back(right);
            num++;
        }
        
        //set n to the next node in open (the one with the lowest heuristic value)
        n = open.top();

        //clear screen
        std::cout << "\033[2J\033[1;1H";
    }

    //print solution
    n.printBoard();

    return n; //return solution
}


int main() {
    //Board initial(std::cin); //initial state

    //hard-coded initial state (for debugging)
    int d[4][4] = { {10,5,11,3},
                    {9,4,0,6},
                    {13,7,12,14},
                    {2,1,15,8} };

    Board initial(d); //initial state

    std::cout << "\nInitial state: " << std::endl;
    initial.printBoard(); //print initial state

    std::cout << "\nGenerating solution... " << std::endl;

    int numNodes; //number of nodes generated

    Board solution = best_first_search(initial, numNodes); //find solution

    if (solution == Board()) //if solution is empty Board object (solution not found), return 
        return 0;
    
    //output results (if solution is found)
    std::cout << "Solution path (with states): " << std::endl;
    //solution.printStatePath(); //print path with states 
    std::cout << "# nodes generated: " << numNodes << std::endl;
    std::cout << "Length of solution path: " << solution.g() << std::endl;
    std::cout << "Solution path (with operators): " << std::endl;
    //printPath(solution, 1); //print path with operators

    return 0;
}
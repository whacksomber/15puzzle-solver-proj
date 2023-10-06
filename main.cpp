#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <chrono> //for timing
#include <limits>
#include <unordered_set> //for hash table
#include "Board.h" //include Board class

//TODO: on pc, remove launch.json from .gitignore (for debugging) (make it an exception to everything in .vscode/*)

struct BoardHasher {
    std::size_t operator()(const Board& b) const {
        std::size_t h = 0;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                h = h * 31 + std::hash<int>()(b.state[i][j]);
        return h;
    }
};

/* best-first search algorithm, returns the solution node
 * is = initital state
 * num = number of nodes generated
 */
Board best_first_search(Board is, int &num) {
    Board* n = new Board(is); //create pointer to board object, set to initial state on first iteration
    num = 1;      //set number of nodes generated to 1 (for initial state)

    //create two sets of boards, depending on whether nodes are yet to be visited (open) or have already been visited (closed)
    std::priority_queue<Board> open; //priority queue of boards, sorted by heuristic value
    std::vector<Board> open_vec;     //vector of open nodes, to keep track of duplicates
    
    std::unordered_set<Board, BoardHasher> closed; //hash table of boards, to keep track of duplicates

    open.push(*n); //push initial state onto priority queue
    open_vec.push_back(*n); //push initial state onto vector

    int lower_bound = n->get_heuristic(); //lower bound of f values of nodes in open

    while (!n->isGoal()) { //while goal has not yet been found
        if (open.empty()) { //if open is empty
            std::cout << "No solution found." << std::endl;
            return Board(); //return empty Board object
        }

        //remove n from open and add it to closed
        open.pop();
        open_vec.erase(std::remove(open_vec.begin(), open_vec.end(), *n), open_vec.end());
        closed.push_back(*n);

        Board children [4] = {n->moveUp(), n->moveDown(), n->moveLeft(), n->moveRight()}; //array of n's children [up, down, left, right

        for (Board child : children) {
            if (child != *n && std::find(closed.begin(), closed.end(), child) == closed.end() &&
                std::find(open_vec.begin(), open_vec.end(), child) == open_vec.end()) {
                    if (child.get_heuristic() > lower_bound) //if child's heuristic value is less than lower bound
                        continue; //skip child
                    open.push(child);
                    open_vec.push_back(child);
                    num++;
            }
        }

        //set current pointer to point to the top of the priority queue
        Board* temp = new Board(open.top()); //create new Board object, set to top of priority queue
        n = temp;
    }

    return *n; //return solution node
}

int main() {
    auto start_time = std::chrono::high_resolution_clock::now(); //start timer

    Board initial(std::cin); //initial state

    std::cout << "\nInitial state: " << std::endl;
    initial.printBoard();

    std::cout << "\nGenerating solution... " << std::endl;

    int numNodes; //number of nodes generated

    Board solution = best_first_search(initial, numNodes); //find solution

    if (solution == Board()) //return 0 if no solution found (empty Board object)
        return 0;
    
    //output results (if solution is found)
    std::cout << "Solution path (with states): " << std::endl;
    solution.printStatePath(); //print path with states

    std::cout << "# nodes generated: " << numNodes << std::endl;

    std::cout << "Length of solution path: " << solution.get_g() << std::endl;

    std::cout << "Solution path (with operators): " << std::endl;
    solution.printOperatorPath();

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time); //calculate duration

    std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl;

    return 0;
}
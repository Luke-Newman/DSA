#include <iostream>

#include "Sliding_puzzle.h"

int main() {

    Sliding_puzzle s;
    std::vector<int> initial_state;

    initial_state = s.generate_initial();

    std::cout << "BFS:\n";
    std::cout << "Initial state:\n";
    s.print_state(initial_state);
    s.breadth_first_search(initial_state);
    std::cout << "States deleted: " << deleted << std::endl;

    std::cout << "\nA*:\n";
    std::cout << "Initial state:\n";
    s.print_state(initial_state);
    s.a_star_search(initial_state);
    std::cout << "States deleted: " << deleted << std::endl;

    return 0;
}
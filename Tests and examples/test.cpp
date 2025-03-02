#include <iostream>
#include "CelluarAutomatonLibrary.hpp"

int main() {
    gtd::CelluarAutomaton<int,4,7> automaton{}; // Creating a CelluarAutomaton object
    std::cout << automaton[1][2] << std::endl; // Indexing a CelluarAutomaton object
    return 0;
}
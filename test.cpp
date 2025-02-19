#include <iostream>
#include "CelluarAutomatonLibrary.hpp"

int main() {
    gtd::CelluarAutomaton<int,4,7> automaton{};
    std::cout << automaton[1][2] << std::endl;
    return 0;
}
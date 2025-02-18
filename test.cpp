#include <iostream>
#include "CelluarAutomatonLibrary.hpp"

int main() {
    gtd::CelluarAutomaton<int,3,3> automat{};
    std::cout << automat[0][0] << std::endl;
    std::cout << automat[2][2] << std::endl;
    return 0;
}
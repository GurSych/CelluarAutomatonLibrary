#include <iostream>
#include "CelluarAutomatonLibrary.hpp"

bool life_rule(std::pair<const bool*,std::array<const bool*,8>> p) {
    unsigned int neighbours_num{};
    for(const bool* n_cell : p.second)
        if(n_cell != nullptr) neighbours_num += (*n_cell ? 1u : 0u);
    if(*p.first && (neighbours_num < 2u || neighbours_num > 3u)) return false;
    else if(!*p.first && neighbours_num == 3u) return true;
    return *p.first;
}
std::string draw_rule(bool value) {
    return (value ? "#" : ".");
}

int main() {
    gtd::CelluarAutomaton<bool,8,14> automaton{false,life_rule};
    automaton[1][1] = true;
    automaton[2][3] = true;
    automaton[3][1] = true; automaton[3][2] = true; automaton[3][3] = true;
    std::string input{};
    do {
        std::cout << automaton.draw(draw_rule) << std::endl;
        automaton.step();
        std::getline(std::cin,input);
    } while(input != "quit");
    return 0;
}
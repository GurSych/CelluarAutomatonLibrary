#include <iostream>
#include <string>
#include "CelluarAutomatonLibrary.hpp"

bool life_rule(std::pair<bool*,std::array<bool*,8>> p) {
    unsigned int neighbours_num{};
    for(bool* n_cell : p.second) {
        if(n_cell == nullptr) neighbours_num += 0u;
        else neighbours_num += (*n_cell ? 1u : 0u);
    }
    if(*p.first && (neighbours_num < 2u || neighbours_num > 3u)) return false;
    else if(!*p.first && neighbours_num == 3u) return true;
    return *p.first;
}
std::string draw_rule(bool value) {
    return (value ? "#" : ".");
}

int main() {
    gtd::CelluarAutomaton<bool,14,14> automaton{life_rule};
    automaton[1][1] = true;
    automaton[2][3] = true;
    automaton[3][1] = true; automaton[3][2] = true; automaton[3][3] = true;
    std::string input{};
    do {
        automaton.step();
        std::cout << automaton.draw(draw_rule) << std::endl;
        std::cin >> input;
    } while(input != "quit");
    return 0;
}
#include <iostream>
#include <CelluarAutomatorLibrary.hpp>

int main() {
    gtd::CelluarAutomator<int,3,3> automat{};
    std::cout << automat[0][0] << std::endl;
    std::cout << automat[2][2] << std::endl;
    return 0;
}
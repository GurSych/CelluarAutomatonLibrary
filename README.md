# CelluarAutomatonLibrary
Celluar Automaton Library is my C++ library for easily creating various celluar automatons

# Using Library
> Library is in development so this section is updated together with library itself

**Library connection:**

```cpp
#include "CelluarAutomatonLibrary.hpp"
```

> [!NOTE]
> All library classes are in gtd:: namespace

**Creating simple CelluarAutomaton object:**

```cpp
gtd::CelluarAutomaton<int,4,7> automaton{}; // gtd::CelluarAutomaton<type,rows,columns>
```

| Value   | type   | meaning                     |
|---------|--------|:----------------------------|
| type    | type   | type of each cell's value   |
| rows    | size_t | number of rows              |
| columns | size_t | number of cells in each row |

Code upper creates celluar automaton that could be represented like:
```
int int int int int int int
int int int int int int int
int int int int int int int
int int int int int int int
```

> [!WARNING]
> Size of each axis must be in range of size_t type and be 3 or larger

**Indexing CelluarAutomaton object:**<br>
Map in CelluarAutomaton has two dimantions so you should use double indexing operator<br>
Indexing starts from zero value

```cpp
gtd::CelluarAutomaton<int,3,4> automaton{};
automaton[1][2] = 7; // automaton[row][column]
```

Code upper creates celluar automaton that could be represented like:
```
0 0 0 0
0 0 7 0
0 0 0 0
```

**Setup your CelluarAutomaton:**<br>
_This section hasn't been done yet =(_ <br>
_But it will be written very soon!_

**Equating CelluarAutomaton objects:**<br>
You can using == and != operators to check equality of two CelluarAutomaton objects

> [!WARNING]
> This method won't work if the type you chose doesn't have this operators overloadings

<details>
<summary>Code example</summary>

```cpp
std::cout << std::boolalpha;
gtd::CelluarAutomaton<int,4,7> automaton1{};
gtd::CelluarAutomaton<int,4,7> automaton2{};
std::cout << automaton1 == automaton2 << ' ' << automaton1 != automaton2 << std::endl; // true false
automaton2[1][2] = 7;
std::cout << automaton1 == automaton2 << ' ' << automaton1 != automaton2 << std::endl; // false true
```

</details>

# Conway's Game of Life in less than 30 lines of code
Code bellow is a simple exemple of setuping your automaton using this library. It creates 8x14 bool-type automaton with Game of Life rule. Then it adds there a 'glider' structure and after starts a do-while cycle with calling step() and draw(_rule_) methods.

```cpp
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
    gtd::CelluarAutomaton<bool,8,14> automaton{life_rule};
    automaton[1][1] = true;
    automaton[2][3] = true;
    automaton[3][1] = true; automaton[3][2] = true; automaton[3][3] = true;
    std::string input{};
    do {
        std::cout << automaton.draw(draw_rule) << std::endl;
        automaton.step();
        std::cin >> input;
    } while(input != "quit");
    return 0;
}
```

```
.............. .............. .............. .............. .............. ..............
.#............ .............. .............. .............. .............. ..............
...#.......... .#.#.......... ...#.......... ..#........... ...#.......... ..............
.###.......... ..##.......... .#.#.......... ...##......... ....#......... ..#.#.........
.............. ..#........... ..##.......... ..##.......... ..###......... ...##.........
.............. .............. .............. .............. .............. ...#..........
.............. .............. .............. .............. .............. ..............
.............. .............. .............. .............. .............. ..............
```


<details>
<summary>How does it work?</summary>
This section hasn't been done yet =(<br>
But it will be written very soon!
</details>

# Plans and ideas
Library will provide you range of celluar automaton classes which you will be able to setup using lambda-function or even using simple template-language<br>
Added and finished classes:
- [x] CelluarAutomaton
- [ ] CelluarAutomaton1D
- [ ] CelluarAutomaton3D

Library features:
- [x] Automaton setup using lambda-function
- [ ] Automaton setup using special language

# Versions
**Current version: 1.0.1**<br>
+1.x.x
  + 1.0.x
    + 1.0.1 - base functional of CelluarAutomaton is finished, added feature of setuping automaton rule via a function
+ 0.x.x
  + 0.0.x
    + 0.0.1 - added simple CelluarAutomaton class with indexing operator overload

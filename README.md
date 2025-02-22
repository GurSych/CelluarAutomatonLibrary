# CelluarAutomatonLibrary
Celluar Automaton Library is my C++ library for easily creating various celluar automatons

# Using Library
> Library is in development so this section is updated together with library itself

### Library connection

```cpp
#include "CelluarAutomatonLibrary.hpp"
```

> [!NOTE]
> All library classes are in gtd:: namespace

### Creating simple CelluarAutomaton object

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

### Indexing CelluarAutomaton object
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

 Size values are saved in `y_size` and `x_size` const fields

## Setup your CelluarAutomaton
Setuping rules is the most important part of any celluar automaton. For doing this you should create special _rule-function_ <br>
Your _rule-function_ for `CelluarAutomaton<T,_,_>` must return `T` type and take one argument: `std::pair<T*,std::array<T*,8>>` type (std::pair with < T-pointer and std::array with < 8 T-pointers > >) <br>
_Rule-function_ is called for each cell in your automaton, it must analize input data and then **return** new (or the same) value for the cell

> [!CAUTION]
> **Do not change cell's value via pointer!** You should return value via your _rule-function_

The first element of the pair argument is the pointer to the cell we check, the second one is the array of pointers to this cell's neighbour-cells. Indexing of this array is fixed and the same for every cell:
```
0 1 2
3   4
5 6 7
```

> [!WARNING]
> Neighbour-cell's pointer is `nullptr` if the cell we check is on the border of automaton's map and such neighbour doesn't exist<br>You should always check that pointer isn't `nullptr`

This _rule-function_ can be send into object's constructor, be changed via change_rule(_rule_) method and be used via simple step() method. You can also use this rule separately with step(_rule_) method. 

### What to do if you need to check more neighbours?

> You should change a bit your _rule-function_ - put the new number of neighbours you need.

Standart rule-function gets `std::pair<T*,std::array<T*,8>>` argument, your new rule must get `std::pair<T*,std::array<T*,_>>` ( `_` is a place for the new neighbours number).

> [!NOTE]
> You can't save this _rule-function_ in object, you can only use it with step(_rule_) method

You can't use any number of neighbours. To know correect number you should calculate this mathematical expression: $(((layers*2)+1)^2)-1$ or use `rule_arg_arr_size` static method of CelluarAutomaton class<br>
Layers - how many neighbourhood cubes we check. One layer is standart - that's a 3x3 cube with 8 neighbours, two layers - 5x5 cube with 24 neighbours, three layers - 7x7 cube with 48 neighbours etc...<br>
Array of neighbours will stil have fixed indexing like this:

```
1 layer: 0 1 2   2 layers:  0  1  2  3  4   3 layers:  0  1  2  3  4  5  6
         3   4              5  6  7  8  9              7  8  9 10 11 12 13
         5 6 7             10 11    12 13             14 15 16 17 18 19 20
                           14 15 16 17 18             21 22 23    24 25 26
                           19 20 21 22 23             17 28 29 30 31 32 33
                                                      34 35 36 37 38 39 40
                                                      41 42 43 44 45 46 47
```

there're also examples of such rules' function-signatures:

```cpp
T rule_of_two_layers(std::pair<T*,std::array<T*,24>>);
T rule_of_three_layers(std::pair<T*,std::array<T*,48>>);
```

### Drawing your CelluarAutomaton object
You can get `std::string` object that would represent your automaton's map using draw(_rule_) method. Rule is a function that takes `T` and returns `std::string` - string that will be added to the main one

### Equating CelluarAutomaton objects
You can using == and != operators to check equality of two CelluarAutomaton objects

> [!WARNING]
> This operators won't work if the type you chose doesn't have this operators overloadings

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
#include "CelluarAutomatonLibrary.hpp"

bool life_rule(std::pair<bool*,std::array<bool*,8>> p) {
    unsigned int neighbours_num{};
    for(bool* n_cell : p.second)
        if(n_cell != nullptr) neighbours_num += (*n_cell ? 1u : 0u);
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
    
> Before reading this please check out ['Setup your CelluarAutomaton'](#setup-your-celluarautomaton) section

`1-2 lines` Connecting libraries<br>
`4-11 lines` Initializing Game of Life _rule-function_ <br>
`12-14 lines` Initializing drawing rule-function<br>
`17 line` Initializing CelluarAutomaton object<br>
`18-20 lines` Adding a 'glider' structure<br>
`23 line` Calling draw(_rule_) method<br>
`24 line` Calling step() method<br>
<br>**What actually happends in our _rule-function_?**<br>
Our function gets `std::pair<bool*,std::array<bool*,8>>` type argument as `p`<br>
Firstly I initialise `neighbours_num` variable that will represent number of live-cells near of the cell. Then I start count these neighbours using this cycle

```cpp
for(bool* n_cell : p.second) // check each neighbour-cell
    if(n_cell != nullptr) neighbours_num += (*n_cell ? 1u : 0u); // check that pointer isn't nullptr and then add one if the cell is live
```

When I finally know about cell's neighbourhood, I apply Game of Life rules

```cpp
if(*p.first && (neighbours_num < 2u || neighbours_num > 3u)) return false; // kill the cell if it's live and neighbours_num not in range [2;3]
else if(!*p.first && neighbours_num == 3u) return true; // ressurect the cell if it's dead and neighbours_num is 3
return *p.first; // cell saves its state if we doesn't change it upper
```

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
**Current version: 2.0.2**<br>
+ 2.x.x
  + 2.0.x
    + 2.0.2 - endless_map option is done
    + 2.0.1 - added multi-layer rules and few static methods
+ 1.x.x
  + 1.0.x
    + 1.0.2 - rule-function's argument type macro, y_size/x_size and new constructors logic added
    + 1.0.1 - base functional of CelluarAutomaton is finished, added feature of setuping automaton rule via a function
+ 0.x.x
  + 0.0.x
    + 0.0.1 - added simple CelluarAutomaton class with indexing operator overload

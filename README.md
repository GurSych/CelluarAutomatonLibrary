# CelluarAutomatonLibrary

Celluar Automaton Library is my C++ library for easily creating various celluar automatons

# Using Library

> Library is in development so this section is updated together with library itself

### Library connection

```cpp
#include "CelluarAutomatonLibrary.hpp"
```

> [!NOTE]
> All library classes are in `gtd::` namespace

### Creating simple CelluarAutomaton object

```cpp
gtd::CelluarAutomaton<int,4,7> automaton{}; // gtd::CelluarAutomaton<type,rows,columns>
```

| Value   | type   | meaning                     |
| ------- | ------ | :-------------------------- |
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

<details>
<summary>All constructors</summary>

Default constructor just initializes cells with default value but there're four more constructors:

+ _Rule-function_ constructor gets your [standard](#setup-your-celluarautomaton) _rule-function_ and saves it (cell-initializing with default value). Example:

```cpp
int rule(std::pair<const T*,std::array<const T*,8>>);
gtd::CelluarAutomaton<int,4,7> automaton{rule}; // gtd::CelluarAutomaton<type,rows,columns>(rule-function)
```

+ Default value constructor gets value and assigns it to each cell. Example:

```cpp
int value = 17;
gtd::CelluarAutomaton<int,4,7> automaton{value}; // gtd::CelluarAutomaton<type,rows,columns>(standard value)
```

+ Default value + _rule-function_ constructor uses concepts of two previews' ones. Example:

```cpp
int value = 17;
int rule(std::pair<const T*,std::array<const T*,8>>);
gtd::CelluarAutomaton<int,4,7> automaton{value,rule}; // gtd::CelluarAutomaton<type,rows,columns>(standard value, rule-function)
```

+ Object constructor gets another CelluarAutomaton object and copies its map and rule. Example:

```cpp
int value = 17;
int rule(std::pair<T*,std::array<T*,8>>);
gtd::CelluarAutomaton<int,4,7> automaton{value,rule};
gtd::CelluarAutomaton<int,4,7> new_automaton{automaton}; // gtd::CelluarAutomaton<type,rows,columns>(CelluarAutomaton object)
```

</details>

### Indexing CelluarAutomaton object

Map in CelluarAutomaton has two dimantions so you should use double indexing operator <br>
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

Setting up rules is the most important part of any celluar automaton. For doing this you should create special _rule-function_ <br>
Your _rule-function_ for `CelluarAutomaton<T,_,_>` must return `T` type and  one argument: `std::pair<const T*,std::array<const T*,8>>` type (std::pair with < const T-pointer and std::array with < 8 const T-pointers > >) <br>
_Rule-function_ is called for each cell in your automaton, it must analize input data and then **return** new (or the same) value for the cell

<!-- > [!CAUTION]
> **Do not change cell's value via pointer!** You should return value via your _rule-function_ -->

The first element of the pair argument is the pointer to the cell we check, the second one is the array of pointers to this cell's neighbour-cells. Indexing of this array is fixed and the same for every cell:

```
0 1 2
3   4
5 6 7
```

> [!WARNING]
> Neighbour-cell's pointer is `nullptr` if the cell we check is on the border of automaton's map and such neighbour doesn't exist <br>You should always check that pointer isn't `nullptr`

This _rule-function_ can be send into object's constructor, be changed via change_rule(_rule_) method and be used via simple step() method. You can also use this rule separately with step(_rule_) method.

### What to do if you need to check more neighbours?

> You should change a bit your _rule-function_ - put the new number of neighbours you need.

Standard rule-function gets `std::pair<const T*,std::array<const T*,8>>` argument, your new rule must get `std::pair<const T*,std::array<const T*,_>>` ( `_` is a place for the new neighbours number).

> [!NOTE]
> You can't save this _rule-function_ in object, you can only use it with step(_rule_) method

You can't use any number of neighbours. To know correect number you should calculate this mathematical expression: $(((layers*2)+1)^2)-1$ or use `rule_arg_arr_size` static method of CelluarAutomaton class <br>
Layers - how many neighbourhood squares we check. One layer is standard - that's a 3x3 square with 8 neighbours, two layers - 5x5 square with 24 neighbours, three layers - 7x7 square with 48 neighbours etc...<br>
Array of neighbours will still have fixed indexing like this:

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
T rule_of_two_layers(std::pair<const T*,std::array<const T*,24>>);
T rule_of_three_layers(std::pair<const T*,std::array<const T*,48>>);
```

### Custom catching neighbour rule

There's step(_rule_,_rule_) method that allows you send one more rule: rule of catching neighbours. This rule is a `std::array<std::pair<long long int,long long int>,Size>` type argument, there each pair is y-axis and x-axis values those are added to dots coordinates to get neighbour coordinates. First _rule-function_ must get the same size of array: `std::pair<const T*,std::array<const T*,Size>><br>`
Example of indexing neighbour array for

```cpp
T rule(std::pair<const T*,std::array<const T*,5>>);
std::array<std::pair<long long int,long long int>,5> neighbor_rule{
    std::pair<long long int,long long int>{-1,1},{2,-2},{1,1},{0,-1},{-2,-1}
};
automaton.step(rule,neighbor_rule); // automaton -> gtd::CelluarAutomaton<T,_,_>
```

around `■` cell:

```
· 4 · · ·
· · · 0 ·
· 3 ■ · · 
· · · 2 ·
1 · · · ·
```

> [!TIP]
> You can use `GTD_CA_NEIGHB_RULE(Size)` macro for `std::array<std::pair<long long int,long long int>,Size>` type and `GTD_CA_NEIGHB_PAIR` macro for `std::pair<long long int,long long int>` type

### Drawing your CelluarAutomaton

You can get a `std::string` object that would represent your automaton's map using draw(_rule_) method. Rule is a function that gets `T` and returns `std::string` - string that will be added to the main one
This's an example of a simple darw-rule fuinction that gets int from CelluarAutomaton<int,\_,\_> which can have only one-digit numbers and returns it as a string:

```cpp
std::string draw_rule(int number) {
    return std::to_string(number);
}
```

### Operators overloadings

**Equating CelluarAutomaton objects** <br>
You can use == and != operators to check equality of two CelluarAutomaton objects

> [!WARNING]
> This operators won't work if the type you chose doesn't have these operators overloadings

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

**Assignment operator** <br>
You can use = operator to get map and rule from another CelluarAutomaton object

# Conway's Game of Life in less than 30 lines of code

Code bellow is a simple example of setting up your automaton using this library. It creates 8x14 bool-type automaton with Game of Life rule. Then it adds there a 'glider' structure and after starts a do-while cycle with calling step() and draw(_rule_) methods.

```cpp
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
<summary>How it works?</summary>

> Before reading this please check out [&#39;Setup your CelluarAutomaton&#39;](#setup-your-celluarautomaton) section

`1-2 lines` Connecting libraries<br>
`4-11 lines` Initializing Game of Life _rule-function_ <br>
`12-14 lines` Initializing drawing rule-function<br>
`17 line` Initializing CelluarAutomaton object<br>
`18-20 lines` Adding a 'glider' structure<br>
`23 line` Calling draw(_rule_) method<br>
`24 line` Calling step() method<br>
<br>**What actually happends in our _rule-function_?**<br>
Our function gets `std::pair<const bool*,std::array<const bool*,8>>` type argument as `p`. That's a cell and its neighbours.<br>

Firstly I initialise `neighbours_num` variable that will represent number of live-cells near of the cell. Then I start count these neighbours using this cycle

```cpp
for(const bool* n_cell : p.second) // check each neighbour-cell
    if(n_cell != nullptr) neighbours_num += (*n_cell ? 1u : 0u); // check that pointer isn't nullptr and then add one if the cell is live
```

When I finally know about cell's neighbourhood, I apply Game of Life rules

```cpp
if(*p.first && (neighbours_num < 2u || neighbours_num > 3u)) return false; // kill the cell if it's live and neighbours_num not in range [2;3]
else if(!*p.first && neighbours_num == 3u) return true; // ressurect the cell if it's dead and neighbours_num is 3
return *p.first; // cell saves its state if we doesn't change it upper
```

**What actually happends in our draw-function?**<br>
Our function gets `bool` type argument as `value`. Thas's a cell's value.<br>
Then it returns to automaton `#` if cell is live and `.` if dead.

</details>

# Plans and ideas

Library will provide you with a range of celluar automaton classes which you will be able to setup using lambda-function or even using simple template-language <br>
Added and finished classes:

- [X] CelluarAutomaton
- [ ] CelluarAutomaton1D
- [ ] CelluarAutomaton3D

Library features:

- [X] Automaton setup using lambda-function
- [ ] Automaton setup using special language

# Versions

**Current version: 2.1.1** <br>

+ 2.x.x
  + 2.1.x
    + 2.1.1 - neighbour catching rule feature and new constructors
  + 2.0.x
    + 2.0.2 - endless_map option is done
    + 2.0.1 - added multi-layer rules and few static methods
+ 1.x.x
  + 1.0.x
    + 1.0.2 - rule-function's argument type macro, y_size/x_size and new constructors logic added
    + 1.0.1 - base functional of CelluarAutomaton is finished, added feature of setting up automaton rule via a function
+ 0.x.x
  + 0.0.x
    + 0.0.1 - added simple CelluarAutomaton class with indexing operator overload

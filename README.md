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

**Equating CelluarAutomaton objects:**<br>
You can using == and != operators to check equality of two CelluarAutomatons objects

<details>
<summary>Code example</summary>

```cpp
std::cout << std::boolalpha;
gtd::CelluarAutomaton<int,4,7> automaton1{};
gtd::CelluarAutomaton<int,4,7> automaton2{};
std::cout << automaton1 == automaton2 << ' ' << automaton1 != automaton2 << std::endl; // true false
automaton[1][2] = 7;
std::cout << automaton1 == automaton2 << ' ' << automaton1 != automaton2 << std::endl; // false true
```

</details>

# Plans and ideas
Library will provide you range of celluar automaton classes which you will be able to setup using lambda-function or even using simple template-language<br>
Added and finished classes:
- [ ] CelluarAutomaton
- [ ] CelluarAutomaton1D
- [ ] CelluarAutomaton3D

Library features:
- [ ] Automaton setup using lambda-function
- [ ] Automaton setup using special language

# Versions
**Current version: 0.0.1**<br>
+ 0.x.x
  + 0.0.x
    + 0.0.1 - added simple CelluarAutomaton class with indexing operator overload

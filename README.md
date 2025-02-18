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
gtd::CelluarAutomaton<int,4,7> automate{}; // gtd::CelluarAutomaton<type,raws,columns>
```

| Value   | type   | meaning                     |
|---------|--------|:----------------------------|
| type    | type   | type of each cell's value   |
| raws    | size_t | number of raws              |
| columns | size_t | number of cells in each raw |

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
indexing starts from zero value

```cpp
gtd::CelluarAutomaton<int,3,4> automate{};
automat[1][2] = 7; // automat[raw][column]
```

Code upper creates celluar automaton that could be represented like:
```
0 0 0 0
0 0 7 0
0 0 0 0
```

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

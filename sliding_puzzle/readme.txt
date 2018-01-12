/****************************************************************************************
S L I D I N G  T I L E  P U Z Z L E  P R O B L E M
****************************************************************************************/

The sliding-tile puzzle consists of three black tiles, three white tiles,
and an empty space in the configuration shown in the following figure:

+---------------------------+
| B | B | B |   | W | W | W |
+---------------------------+

The initial state has a random tile configuration

The puzzle has two legal moves with associated costs:
- A tile can move into an adjacent empty location. This move has a cost of 1. 
- A tile can also hop over one or two other tiles into the empty position. This move has a cost
equal to the number of tiles jumped over. 

The goal is to have all white tiles to the left of all black tiles. The position of the blank is not important.

This solution implements a breadth-first search algorithm and an A* algorithm, then compares the number of
states visited to find the goal, an the path cost of reaching the solution.


/****************************************************************************************
Output Example
****************************************************************************************/

BFS:
Initial state:
 --- --- --- --- --- --- ---
| B | W | B | W | B | W |   |
 --- --- --- --- --- --- ---

Goal achieved after checking 119 states:
 --- --- --- --- --- --- ---
| W | W | W | B | B |   | B |
 --- --- --- --- --- --- ---

Solution Path:
 --- --- --- --- --- --- ---
| B | W | B | W |   | W | B |
 --- --- --- --- --- --- ---
Path cost: 1, Heuristic: 0

 --- --- --- --- --- --- ---
| B | W |   | W | B | W | B |
 --- --- --- --- --- --- ---
Path cost: 2, Heuristic: 0

 --- --- --- --- --- --- ---
|   | W | B | W | B | W | B |
 --- --- --- --- --- --- ---
Path cost: 3, Heuristic: 0

 --- --- --- --- --- --- ---
| W | W | B |   | B | W | B |
 --- --- --- --- --- --- ---
Path cost: 5, Heuristic: 0

 --- --- --- --- --- --- ---
| W | W |   | B | B | W | B |
 --- --- --- --- --- --- ---
Path cost: 6, Heuristic: 0

 --- --- --- --- --- --- ---
| W | W | W | B | B |   | B |
 --- --- --- --- --- --- ---
Path cost: 8, Heuristic: 0

Total path cost: 25
States deleted: 136

A*:
Initial state:
 --- --- --- --- --- --- ---
| B | W | B | W | B | W |   |
 --- --- --- --- --- --- ---

Goal achieved after checking 10 states:
 --- --- --- --- --- --- ---
| W | W | W | B | B |   | B |
 --- --- --- --- --- --- ---

Solution Path:
 --- --- --- --- --- --- ---
| B | W | B | W |   | W | B |
 --- --- --- --- --- --- ---
Path cost: 1, Heuristic: 26

 --- --- --- --- --- --- ---
| B | W |   | W | B | W | B |
 --- --- --- --- --- --- ---
Path cost: 2, Heuristic: 23

 --- --- --- --- --- --- ---
| B | W | W |   | B | W | B |
 --- --- --- --- --- --- ---
Path cost: 3, Heuristic: 23

 --- --- --- --- --- --- ---
|   | W | W | B | B | W | B |
 --- --- --- --- --- --- ---
Path cost: 5, Heuristic: 21

 --- --- --- --- --- --- ---
| W | W |   | B | B | W | B |
 --- --- --- --- --- --- ---
Path cost: 6, Heuristic: 23

 --- --- --- --- --- --- ---
| W | W | W | B | B |   | B |
 --- --- --- --- --- --- ---
Path cost: 8, Heuristic: 20

Total path cost: 25
States deleted: 162

Process finished with exit code 0

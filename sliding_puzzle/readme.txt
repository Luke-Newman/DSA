S L I D I N G  T I L E  P U Z Z L E  P R O B L E M

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
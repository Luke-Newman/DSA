/****************************************************************************************
M I N I  S H R D L U 
****************************************************************************************/

Mini SHRDLU is a puzzle that consists of an n*n vertical board with n^2 - n blocks:

   --- --- ---
  |   |   | 6 |
  | 3 |   | 5 |
  | 4 | 2 | 1 |
   --- --- ---
   
The user can select any number of atom goals which consist of assigning a blocks 
destination (row, column), each block may only be assigned one goal.

The user can select mini_SHRDLU to find a disjunctive solution, or a conjunctive solution, i.e. The problem is solved if a single atom goal is achieved, or if and only if all atom goals
are achieved.

For each goal given, mini_SHRDLU will generate all available actions and assign each a heuristic. The action with the highest weight is selected as the move. One issue with finding
a conjunctive solution was that blocks could become trapped below a block that had already been solved on top (and locked in place). This was avoided by generating a goal state that 
included the users goals, then filled in the remaining blocks and also made them atom goals:

user has selected the atom goals:

   --- --- ---
  |   |   | 6 |
  | 3 |   | 5 |
  |   |   |   |
   --- --- ---
   
remaining blocks filled in and set as goal state:

   --- --- ---
  |   |   | 6 |
  | 3 |   | 5 |
  | 4 | 2 | 1 |
   --- --- ---
 
/****************************************************************************************
B A C K W A R D  C H A I N I N G  R E A S O N I N G
****************************************************************************************/

Backward chaining reasoning is one of the major reasoning mechanisms
in most knowledge based systems.

This system is able to deal with inference rules in the form:
Pv^...^ Pk => P
Pv...vPk => P

The knowledge base is a finite set formulas of form of:
P1^P2^...^Pk => P,
PvP2v...vPk => P, and
P

where the third formula P is a special case of the first two formulas in which
the body of the proposition empty (i.e. it is a fact / True).

Then user can test if a primitive fact Q can be
inferred from your knowledge base.


Backward chaining reasoning is a search problem, and as such search tree is generated during
the reasoning process that can cooperate both conjunct and disjunct features.



Suppose your knowledge base includes:
AvB=>E
A^B=>D
D^E=>F
B^E=>F
A
B
C

If user wants to test if F can be inferred from this knowledge base,
a search tree is built during the reasoning:

           (F)
            /\
           /  \
          /    \
      (D,E) (B,E)
      /  |\
     /   | \
    /    |  \
(A,B)   (A) (B)
 | |     |
 | |     |
 | |     |
 T T     T
 
In the above tree, a node with the form (A,B) indicates
that both A and B should be achieved in the knowledge base
in order to achieve its parent. A node with the form (A)
indicates that A should be achieved in order to achieve its
parent. Further, to achieve a parent node, we only needs to achieve
one of its children. Therefore, a depth first search should be used
in the search procedure.

For example, to achieve F, we only need to achieve node (D,E) or
node (B,E). Then by using depth first search, we try to achieve
node (D,E). That means we need to achieve both D and E. To achieve D, we need
to achieve node (A,B), and to achieve E, we need to achieve node
(A) or node (B).


/****************************************************************************************
Output Example:
Subject is the current variable we all looking to find true
****************************************************************************************/

The Knowledge Base can only accept facts in the format:
P1^P2 ... ^Pk=>P, or
P1vP2 ... vPk=>P, or
P

Please input your knowledge base, press enter twice to finish.
A^B^C^D=>Z
Input Accepted
EvF=>A
Input Accepted
G^H=>E
Input Accepted
I^J=>G
Input Accepted
JvK=>H
Input Accepted
I
Input Accepted
J
Input Accepted
B
Input Accepted
LvM=>C
Input Accepted
L
Input Accepted
i^J^B^y=>D
Input Accepted
y
Input Accepted

Knowledge base:
LM=>C (OR)
JK=>H (OR)
IJBY=>D (AND)
ABCD=>Z (AND)
EF=>A (OR)
GH=>E (AND)
IJ=>G (AND)

Database:
Y
I
J
B
L

Enter query (single alpha character), digit to quit.
c
Subject: C
Subject: M
True

Enter query (single alpha character), digit to quit.
z
Subject: Z
Subject: D
Subject: A
Subject: F
Subject: E
Subject: H
Subject: K
Subject: G
Subject: E
Subject: A
Subject: Z
True

Enter query (single alpha character), digit to quit.
d
Subject: D
True

Enter query (single alpha character), digit to quit.
k
Subject: K
False

Enter query (single alpha character), digit to quit.
0
0

Process finished with exit code 0

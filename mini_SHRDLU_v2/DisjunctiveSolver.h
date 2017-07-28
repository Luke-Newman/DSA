//
// Created by Luke on 28/03/2017.
//

#ifndef MINI_SHRDLU_V1_SOLVER_H
#define MINI_SHRDLU_V1_SOLVER_H

#include <vector>
#include <list>
#include <queue>

#include "AtomGoal.h"
#include "State.h"
#include "Action.h"

/// I chose to implement different solver and heuristic methods for disjunctive and conjunctive goals,
/// as the disjunctive solver is much more inefficient if it follows the conjunctive method. Both solvers work

class DisjunctiveSolver {
private:
    vector<AtomGoal> goals;
    vector<State> triedMoves;
    int k, n, solved;
    State* currentState;
    Action* action;
    int HEURISTIC_MAX;

public:

    DisjunctiveSolver(vector<Cell> initial, vector<AtomGoal> goals, int k, int n) { // TODO make members of solver, do not pass from SHRDLU
        this->k = k;
        this->n = n;
        this->HEURISTIC_MAX = n*n*n*n;
        this->solved = 0;
        this->goals = goals;
        currentState = new State(initial, k, n);
        action = new Action(n);
    }

    virtual ~DisjunctiveSolver();

    void solve();
    vector<State> getAvailableActions();
    State getBestAction();
    bool moveTried(vector<Cell> s);


};

DisjunctiveSolver::~DisjunctiveSolver() {
    delete currentState;
    delete action;
}

void DisjunctiveSolver::solve() {

    int numMoves = 0;
    sort(goals.begin(),goals.end()); // Not priority queue because I need to iterate through goals, complexity similar as I only sort once

    for (int i = 0; i < goals.size(); i++) { // check if any goal is already met
        if (currentState->meetsAtomGoal(goals[i])) {
            cout << "Atom goal " << goals[i].getPrintedGoal() << " achieved" << endl;
            solved = 1;
        }
    }

    if (solved == 1){
        cout << "Solved in " << numMoves << " moves" << endl;
        currentState->displayState(); //TODO quick fix for if user picks achieved goal, tidy up
        return;
    }

    for (int moves = 1; moves <= 100; moves++) {

        currentState->copyState(getBestAction()); // may need to use copy constructor
        numMoves ++;
        cout << "current state passed back to solver with Heuristic: " << currentState->getHeuristic() << endl;
        currentState->displayState();
        cout << endl;

        if (currentState->getHeuristic() == HEURISTIC_MAX){
            cout << "Solved in " << numMoves << " moves" << endl;
            currentState->displayState();
            return;
        }

        if (solved == -1) {
            cout << "No solution found" << endl;
            return;
        }
    }

    cout << "Unable to find a solution in <= 100 moves" << endl;
    return;
}

State DisjunctiveSolver::getBestAction() {

    const vector<State> availableActions = getAvailableActions(); // TODO: move to DisjunctiveSolver class??
    priority_queue<State> evaluatedActions; // TODO: move to solver class??
    State tmpState(currentState->getState(),k,n);

    for (int i = 0; i < availableActions.size(); i++) {
        action->heuristicDisjunctive((State &) availableActions[i], goals); // TODO cleaner implementation
        evaluatedActions.push(availableActions[i]);
    }

    cout << "Number of states in tried moves: " << triedMoves.size() << endl;
    for (int i = (int)evaluatedActions.size(); i>=0; i--) { /// Will never be bigger then int so cast to int
        if (moveTried(evaluatedActions.top().getConstState())) { /// Require an alternate return as State has been declared as const
            evaluatedActions.pop();
        } else {
            triedMoves.push_back(evaluatedActions.top());
            break;
        }
    }

    if (evaluatedActions.empty()) {
        solved = -1;
        return tmpState;
    }

    return evaluatedActions.top();
}

vector<State> DisjunctiveSolver::getAvailableActions() { // TODO: MOVE TO ACTION CLASS! May pass vector<Cell> instead of entire state object

    int sourceIndex, destIndex;
    vector<State> availableMoves;
    vector<int> destIndices = currentState->getVacantColumns();
    vector<int> sourceIndices = currentState->getValidBlocks();
    vector<Cell> s = currentState->getState();

    for (int i = 0; i < destIndices.size(); i++) {
        for (int j = 0; j < sourceIndices.size(); j++ ) {
            State tmpState(currentState->getState(),k,n); /// Must not be pointer to State in solver
            destIndex = destIndices[i];
            sourceIndex = sourceIndices[j];

            if (s[sourceIndex].getCol() != s[destIndex].getCol()) { // must make sure that a block does not choose it's current column as a destination
                tmpState.swapLabel(sourceIndex,destIndex);
                availableMoves.push_back(tmpState);
            }
        }
    }
    return availableMoves;
}

bool DisjunctiveSolver::moveTried(vector<Cell> s) {

    for (int i = 0; i < triedMoves.size(); i++) {

        if(triedMoves[i] == s) { return true; }
    }
    return false;
}



#endif //MINI_SHRDLU_V1_SOLVER_H

//
// Created by Luke on 1/04/2017.
//

#ifndef MINI_SHRDLU_V1_CONJUNCTIVESOLVER_H
#define MINI_SHRDLU_V1_CONJUNCTIVESOLVER_H

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>

#include "AtomGoal.h"
#include "State.h"
#include "Action.h"
#include "GoalStateGenerator.h"

using namespace std;

class ConjunctiveSolver {

private:
    vector<AtomGoal> goals;
    vector<State> triedMoves;
    int k, n, solved;
    int HEURISTIC_MAX;
    State* currentState;
    Action* action;


public:

    ConjunctiveSolver(vector<Cell> initial, vector<AtomGoal> goals, int k, int n) {
        this->k = k;
        this->n = n;
        this->HEURISTIC_MAX = n*n*n*n; // n^4 as I never want to reach from other heuristics other than goal met.
        this->solved = 0;
        currentState = new State(initial, k, n);
        action = new Action(n);

        GoalStateGenerator g(goals, k, n);
        this->goals = g.getGeneratedGoals();
    }

    void solve();
    State getBestAction(AtomGoal currentGoal);
    vector<State> getAvailableActions();
    bool moveTried(vector<Cell> s);

    virtual ~ConjunctiveSolver() {
        delete currentState;
        delete action;
    }
};

void ConjunctiveSolver::solve() {
    int numMoves;
    sort(goals.begin(),goals.end()); // Not priority queue because I need to iterate through goals, complexity similar as I only sort once

    cout << "Generated goals:" << endl; // mostly for test
    for (int i = 0; i < this->goals.size(); i++) {
        cout << goals[i].getPrintedGoal() << endl;
    }
    cout << endl;

    for (numMoves = 0; numMoves <= 100; numMoves++) { // My solution must find within 100 "Human" actions i.e. moves

        if (currentState->allAtomGoalsAchieved(goals)) { // cycles through state and checks all atomgoals met
            currentState->displayState();
            cout << "Solved in " << numMoves << " moves" << endl;
            return;
        }

        currentState->copyState(getBestAction(currentState->getNextAtomGoal(goals))); // cycles backwards through sorted goals to find a bottom-most move that have not been achieved
                                                                                        // also means solver can go back if a solved goal has been moved from bad heuristic
        cout << "current state passed back to solver with Heuristic: " << currentState->getHeuristic() << endl;
        currentState->displayState();
        cout << endl;

        if (solved == -1) {
            cout << "No solution found" << endl;
            return;
        }
    }

    cout << "Unable to find a solution in <= 100 moves" << endl;
    return;
}

State ConjunctiveSolver::getBestAction(AtomGoal currentGoal) {

    const vector<State> availableActions = getAvailableActions();
    priority_queue<State> evaluatedActions; // my queue for storing actions and ordering by heuristic

    State tmpState(currentState->getState(),k,n); // temp as I do not want to alter actual "game state"

    cout << "Searching for atom goal " << currentGoal.getPrintedGoal() << endl;

    if (availableActions.empty()) { // This is not possible to reach with n*n-n blocks but I prefer to handle it
        cout << "No available actions left" << endl;
        solved = -1;
        return tmpState;
    }

    for (int i = 0; i < availableActions.size(); i++) { // Evaluate all legal actions ..
        action->heuristicConjunctive((State &) availableActions[i], currentGoal);
        evaluatedActions.push(availableActions[i]); // Place in priority queue
    }

    while (!evaluatedActions.empty()) {
        if (moveTried(evaluatedActions.top().getConstState())) { // If tried pop the action off queue
            evaluatedActions.pop();
        } else {
            triedMoves.push_back(evaluatedActions.top()); // else push to visited moves
            break;
        }
    }

    if (evaluatedActions.empty()) { // This problem does not occur with improved heuristics
        cout << "No new actions left to evaluate" << endl;
        solved = -1;
        return tmpState;
    }

    if (evaluatedActions.top().getHeuristic() == HEURISTIC_MAX) { // Atom goal achieved, return it
        solved = 1;
        cout << "Atom Goal " << currentGoal.getPrintedGoal() << " achieved" << endl;
        return evaluatedActions.top();
    }

    return evaluatedActions.top(); // This will be the action with the highest heuristic

}

vector<State> ConjunctiveSolver::getAvailableActions() { // TODO: MOVE TO ACTION CLASS! May pass vector<Cell> instead of entire state object

    int sourceIndex, destIndex;
    vector<State> availableMoves;
    vector<int> destIndices = currentState->getVacantColumns(); // get available columns with space
    vector<int> sourceIndices = currentState->getValidBlocks(); // get available top blocks
    vector<Cell> s = currentState->getState();

    for (int i = 0; i < destIndices.size(); i++) {
        for (int j = 0; j < sourceIndices.size(); j++ ) {
            State tmpState(currentState->getState(),k,n); // initialise a tmp state to be pushed into moves vector
            destIndex = destIndices[i];
            sourceIndex = sourceIndices[j];

            // i.e. number of blocks * no. avaliable columns - current blocks column
            if (s[sourceIndex].getCol() != s[destIndex].getCol()) { // must make sure that a block does not choose it's current column as a destination
                tmpState.swapLabel(sourceIndex,destIndex);
                availableMoves.push_back(tmpState);
            }
        }
    }
    return availableMoves;
}

bool ConjunctiveSolver::moveTried(vector<Cell> s) { // to cycle through tried moves, I only copy state vector not entire class

    for (int i = 0; i < triedMoves.size(); i++) {
        if(triedMoves[i] == s) { return true; }
    }
    return false;
}

#endif //MINI_SHRDLU_V1_CONJUNCTIVESOLVER_H

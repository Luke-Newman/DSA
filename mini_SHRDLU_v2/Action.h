//
// Created by Luke on 28/03/2017.
//

#ifndef MINI_SHRDLU_V1_ACTION_H
#define MINI_SHRDLU_V1_ACTION_H

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>

#include "State.h"
#include "AtomGoal.h"
#include "Cell.h"

/// Heuristics are measured differently for disjunctive and conjunctive to help that Program make the optimal decision

class Action {
private:
    int n;
    int HEURISTIC_MAX;
    int HEURISTIC_MID;

public:

    Action(int n) {
        this->n = n;
        this->HEURISTIC_MAX = n*n*n*n;
        this->HEURISTIC_MID = n*n*n;

    }

    void heuristicDisjunctive(State &state, vector<AtomGoal> goals);
    void heuristicConjunctive(State &state, AtomGoal currentGoal);
};

void Action::heuristicDisjunctive(State &state, vector<AtomGoal> goals) {

    int factor1 = 0;
    int factor2 = 0;
    int factor3 = 0;
    int factor4 = 0;
    int targetLabelIndex;
    int targetDestIndex;
    int heuristic = 0;
    int lhsH = 0;
    int rhsH = 0;

    for (int i = 0; i < goals.size(); i++) { // I want to check all atom goals for my disjunctive decision and find the easiest
        targetLabelIndex = state.getTargetBlockIndex(goals[i].getBlock());
        targetDestIndex = state.getIndex(goals[i].getRow(), goals[i].getCol());

        if (state.meetsAtomGoal(goals[i])) {
            state.setHeuristic(HEURISTIC_MAX);
            return;
        }


        factor1 = state.getNumBlocksAbove(targetLabelIndex);
        factor2 = state.getNumBlocksAbove(targetDestIndex);
        factor3 = state.getState()[targetLabelIndex].getRow();
        lhsH = HEURISTIC_MID - factor1 - factor2 - factor3 - factor4;

        if (lhsH > rhsH) { // this will pick the move with the highest heuristic for any atom goal
            rhsH = lhsH;
            heuristic = lhsH;
        }
    }
    state.setHeuristic(heuristic);
}

void Action::heuristicConjunctive(State &state, AtomGoal currentGoal) {

    int targetLabelIndex;
    int targetDestIndex;
    int heuristic = HEURISTIC_MID;

    targetLabelIndex = state.getTargetBlockIndex(currentGoal.getBlock());
    targetDestIndex = state.getIndex(currentGoal.getRow(), currentGoal.getCol());

    if (state.meetsAtomGoal(currentGoal)) { // goal is met set heuristic and return
        state.setHeuristic(HEURISTIC_MAX);
        return;

    }

    if (state.getState()[targetDestIndex].getCol() == state.getState()[targetLabelIndex].getCol()) { // bad to be on top of destination col
        heuristic -= n+n;
    }

    // These are the simplest and best value heuristics, adding more just leads to contradictions in certain states
    if (state.isAtTop(targetLabelIndex)) {
         heuristic += n;

        if (state.vacantCell(targetDestIndex)) {
            heuristic +=n;
        }
    }

    heuristic -= state.getNumBlocksAbove(targetLabelIndex)*n*n;
    heuristic -= state.getNumBlocksAbove(targetDestIndex);

    state.setHeuristic(heuristic);
}

#endif //MINI_SHRDLU_V1_ACTION_H

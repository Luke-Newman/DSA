//
// Created by Luke on 6/04/2017.
//

#ifndef MINI_SHRDLU_V1_GOALSTATEGENERATOR_H
#define MINI_SHRDLU_V1_GOALSTATEGENERATOR_H

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

#include "Cell.h"
#include "State.h"

using namespace std;

class GoalStateGenerator { // I was considering making initial and goal generators extend state class, but this
private:                    // gave too many things I don't need, better to duplicate some functions
    int k, n;
    vector<Cell> state;
    vector<AtomGoal> goals;
    list<int> blocks;


public:

    GoalStateGenerator(vector<AtomGoal> goals, int k, int n) {
        this->k = k;
        this->n = n;
        this->goals = goals;

        generateGoalState();
    }

    void generateGoalState() {
        initialiseState();
        generateBlocks();
        placeGoals();
        fillGaps();
        placeRemainingBlocks();
    }

    void initialiseState() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                state.push_back(Cell(0, i, j));
            }
        }
    }

    void generateBlocks() {
        for (int i = 1; i <= k; i++) {
            blocks.push_back(i);
        }
    }

    void placeGoals() {
        int index;

        for (int i = 0; i < state.size(); i++) {
            for (int j = 0; j < goals.size(); j++) {
                index = getIndex(goals[j].getRow(), goals[j].getCol());

                if (i == index) {
                    state[index].setLabel(goals[j].getBlock());
                    blocks.remove(goals[j].getBlock());
                }
            }
        }
    }

    void fillGaps() {
        int index;

        for (int i = 0; i < n; i++) {
            index = getTopBlock(i);

            if (!isBottom(index)) {
                placeBlocksBelow(index + n);
            }
        }
    }

    void placeBlocksBelow(int index) {
        for (int i = index; i < state.size(); i += n) {
            if (isBottom(i) || blocks.empty()) { break; }

            if (vacantCell(i)) {
                state[i].setLabel(blocks.back());
                goals.push_back(AtomGoal(blocks.back(), state[i].getRow(), state[i].getCol()));
                blocks.pop_back();
            }
        }
    }

    void placeRemainingBlocks() {
        if (blocks.empty()) { return; }

        for (int i = (int) state.size() - 1; i >= 0; i--) {

            if (blocks.empty()) { break; }

            if (state[i].getLabel() == 0) {
                state[i].setLabel(blocks.back());
                goals.push_back(AtomGoal(blocks.back(), state[i].getRow(), state[i].getCol()));
                blocks.pop_back();
            }
        }
    }

    int getIndex(int row, int col) { // Is in state class
        return row * n + col;
    }

    int getTopBlock(int index) { // Is in state class
        int newIndex = 0;

        for (int i = 0; i < state.size(); i += n) {
            newIndex = i + index;

            if (!vacantCell(newIndex)) {
                return newIndex;
            }
        }
        return newIndex;
    }

    bool vacantCell(int index) { return state[index].getLabel() == 0; } // in State Class

    bool isBottom(int index) { return index >= state.size(); }

    vector<AtomGoal> getGeneratedGoals() {
        return this->goals;
    }

};

#endif //MINI_SHRDLU_V1_GOALSTATEGENERATOR_H

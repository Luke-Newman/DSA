//
// Created by Luke on 28/03/2017.
//

#ifndef MINI_SHRDLU_V1_STATEGENERATOR_H
#define MINI_SHRDLU_V1_STATEGENERATOR_H

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

#include "Cell.h"

using namespace std;

class StateGenerator { // I have stuck with this design from the very beginning as it works and is not highest priority
private:                // I would replace recursion with iteration if I had the time to spare
    vector<Cell> state;
    int k,n;
public:

    StateGenerator(int k, int n) {
        this->k = k;
        this->n = n;
        generateState();
    }

    void generateState();
    void generator(vector<int> blocks);
    bool hasBottom(int index);
    int getBottom(int row, int col);
    vector<Cell> getState();
};

void StateGenerator::generateState() {
    vector<int> blocks;

    for (int i = 1; i <= k; i++) { // correct
        blocks.push_back(i);
    }

    random_shuffle(blocks.begin(),blocks.end());

    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            state.push_back(Cell(0,row,col));
        }
    }
    generator(blocks);
}

void StateGenerator::generator(vector<int> blocks) {
    int col;
    int row = 0;
    int index;

    while (!blocks.empty()) { // Dropping blocks down is a human way of thinking, I'm sure there is a better was
        col = (unsigned int)(rand() % n);
        index = row * n + col;

        if (state[index].getLabel() == 0) {
            state[getBottom(row,col)].setLabel(blocks.back()); // recursively finds the bottom, "dropping' blocks down
            blocks.pop_back();
        }
    }
}

int StateGenerator::getBottom(int row, int col) {
    int index = row * n + col;

    if (hasBottom(index+n)) { return index; }

    return getBottom(row+1, col);
}

bool StateGenerator::hasBottom(int index) {
    if (index >= state.size()) { return true; }

    return state[index].getLabel() > 0;
}

vector<Cell> StateGenerator::getState() { return this->state; }

#endif //MINI_SHRDLU_V1_STATEGENERATOR_H

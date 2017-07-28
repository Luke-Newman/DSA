//
// Created by Luke on 28/03/2017.
//

#ifndef MINI_SHRDLU_V1_STATE_H
#define MINI_SHRDLU_V1_STATE_H

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

#include "Cell.h"

using namespace std;

class State {

private:
    int k, n; // maybe not needed later, n is used often though
    vector<Cell> state;
    int heuristic;
public:

    State(vector<Cell> state, int k, int n) {
        this->k = k;
        this->n = n;
        this->state = state;
        this->heuristic = 0;
    }

    State(vector<Cell> state) { /// cant use?
        this->state = state;
    }

    virtual ~State();
    bool operator==(State s); // equality overloaded
    bool operator<(const State rhs) const; // overloaded for priority queue
    bool allAtomGoalsAchieved(vector<AtomGoal> goals); // my condition for solved TODO consider move to solver
    AtomGoal getNextAtomGoal(vector<AtomGoal> goals); // Gets one of the next lowest atom goals in state TODO consider move to solver
    bool meetsAtomGoal(AtomGoal a); // checks a state cell meets an atom goal TODO consider move to solver
    void copyState(State s); // used to copy current state to tmp State, vice versa
    vector<Cell> getState(); // returns state vector
    void setHeuristic(int val);
    int getTargetBlockIndex(int label); // need to find the target block of atom goal in state
    int getCellLabel(int index); // get the label value of a Cell object
    void swapLabel(int index1, int index2); // swap the label data member of two cells
    int getIndex(int row, int col); // converts row,col to index for use with vector
    vector<int> getVacantColumns(); // get columns for available moves
    vector<int> getValidBlocks(); // get blocks for available moves
    bool vacantCell(int index); // cell with no block
    bool emptyColumn(int index); // checks to make an 0(n) case O(1)
    int getNumBlocksAbove(int index); // Used as heuristic
    int getTopBlock(int index); // get the top-most block
    int getTop(int index); // Get the top-most legal index
    bool isTopBlock(int index); // Check if an index is at the top of a column
    bool isAtTop(int index); // used to check if a block/vacant cell is at the top
    bool hasTop(int index); // check if an index has a top
    int getBottom(int index); /// NOTE: the semantics for this function name are ambiguous, this function returns the bottom-most legal index, not the actual bottom of  a column
    bool hasBottom(int index); // check if an index has a label bellow it, or if the index is at the bottom of the column
    int getHeuristic() const;
    void displayState() const;

    const vector<Cell> &getConstState() const{ return this->state; } // required for some STL containers, and for comparison
};

State::~State() {

}

bool State::operator==(State s) {
    for (int i = 0; i < state.size(); i++) {
        if (state[i].getLabel() != s.getState()[i].getLabel()) {
            return false;
        }
    }
    return true;
}

bool State::operator<(const State rhs) const{ return this->heuristic < rhs.getHeuristic(); }

bool State::allAtomGoalsAchieved(vector<AtomGoal> goals) {

    while(!goals.empty()) {
        if(meetsAtomGoal(goals.back())) {
            goals.pop_back();
        } else {
            return false;
        }
    }

    return true;
}

AtomGoal State::getNextAtomGoal(vector<AtomGoal> goals) {

    for (int i = (int)goals.size()-1; i>=0; i--) {
        if (!meetsAtomGoal(goals[i])) {
            return goals[i];
        }
    }
    return goals.front(); // Note, will never be reached as I check for all goals achieved first
}

bool State::meetsAtomGoal(AtomGoal a) {
    int index = getIndex(a.getRow(), a.getCol());
    return state[index].getLabel() == a.getBlock();
}

void State::copyState(State s) {
    this->state = s.getState();
    this->heuristic = s.getHeuristic();
}

vector<Cell> State::getState() { return this->state; }

void State::setHeuristic(int val) { this->heuristic = val; }

int State::getHeuristic() const{ return this->heuristic; }


int State::getTargetBlockIndex(int label) {

    for (int i = 0; i < state.size(); i++) {
        if (state[i].getLabel() == label) {
            return i;
        }
    }
    return -1;
}

int State::getCellLabel(int index) { return state[index].getLabel(); }

void State::swapLabel(int index1, int index2) {
    int tmp = this->state[index1].getLabel();
    this->state[index1].getLabel(this->state[index2].getLabel());
    this->state[index2].setLabel(tmp);
}

int State::getIndex(int row, int col) { return row * n + col; }

vector<int> State::getVacantColumns() { /// fix
    vector<int> cols;
    int index;
    for (int col = 0; col < n; col++) { // n because I only need to skim across the top of "board"
        if (getCellLabel(col) == 0) {
            index = getBottom(col);
            cols.push_back(index);
        }
    }
    return cols;
}

vector<int> State::getValidBlocks() { /// fix
    vector<int> blocks;
    int index;

    for (int col = 0; col < n; col++) {

        if (!emptyColumn(col)) {
            index = getTopBlock(col);

            blocks.push_back(index);
        }
    }
    return blocks;
}

bool State::vacantCell(int index) { return getCellLabel(index) == 0; }

bool State::emptyColumn(int index) { return vacantCell(((n*n - n) + index)); }

int State::getNumBlocksAbove(int index) {
    int num = 0;
    if (isTopBlock(index)) {
        return 0;
    }

    for (int i = index; i > 0 ; i -=n) {

        if (hasTop(i)) {
            num++;
        } else {
            break;
        }
    }
    return num;
}

int State::getTopBlock(int index) { // TODO: I would prefer to implement this way instead of recursion if i have time

    int newIndex = 0;

    for (int i = 0; i < state.size(); i +=n) {
        newIndex = i + index;

        if (!vacantCell(newIndex)) {
            return newIndex;
        }
    }
    return newIndex;
}

int State::getTop(int index) { // TODO: Ugly; fix

    int i = -1;
    if (isTopBlock(index)) {
        return index;
    }

    if (hasTop(index)) {
        i = getTop(index-n);
    }

    if (i == -1) {
        cout << "func getTop failed!" << endl;
    }

    return i; // not necessary but as a fallthrough
}

bool State::isTopBlock(int index) {

    if (index-n < 0 || index < n) {
        return true;
    }
    return state[index-n].getLabel() == 0;
}

bool State::isAtTop(int index) {
    return index < n;

}

bool State::hasTop(int index) {

    if (index-n < 0) {
        return false;
    }
    return state[index-n].getLabel() > 0;
}


int State::getBottom(int index) {

    if (hasBottom(index)) {
        return index;
    }
    return getBottom(index+n);
}

bool State::hasBottom(int index) {

    if (index+n >= state.size()) {
        return true;
    }
    return state[index+n].getLabel() > 0;
}

void State::displayState() const{

    int tmp = 0;
    int tmpRow = 0;

    cout << "     "; // make offset for x axis numbers
    for(int i = 0; i < n; i++) { // loop to generate numbers along x axis
        if (i < 10) {
            cout << i << "   ";
        } else {
            cout << i << "  "; /// for n > 10 board
        }

    }

    cout << "\n   "; // make offset for " ---"

    for(int k = 0; k < n; k++) { // for the number of columns..
        cout << " ---";
    }


    cout << "\n" << tmpRow << "  "; // print corresponding row value with offset


    for (int i = 0; i < state.size(); i++) {

        if (tmp == n ) {

            tmpRow ++;

            cout << endl;

            if (tmpRow < 10) {
                cout << tmpRow << "  ";
            } else {
                cout << tmpRow << " ";
            }

            tmp = 0;
        }
        tmp ++;

        cout << "|";

        if (state[i].getLabel() == 0) {
            cout << "   ";
        } else if (state[i].getLabel() < 10) {
            cout << " " << state[i].getLabel() << " ";
        } else if (state[i].getLabel() < 100){
            cout << " " << state[i].getLabel();
        } else {
            cout << state[i].getLabel();
        }

        if (tmp == n) {
            cout << "|";
        }
    }

    cout << "\n   "; // final offset

    for(int k = 0; k < n; k++) { // for the number of columns..
        cout << " ---"; // print final dashes on last cell
    }

    cout << endl;
}

#endif //MINI_SHRDLU_V1_STATE_H

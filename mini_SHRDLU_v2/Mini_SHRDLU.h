//
// Created by Luke on 28/03/2017.
//

#ifndef MINI_SHRDLU_V1_MINI_SHRDLU_H
#define MINI_SHRDLU_V1_MINI_SHRDLU_H

#include <iostream>
#include <limits>
#include <math.h>
#include <vector>
#include <ctime>

#include "State.h"
#include "Cell.h"
#include "StateGenerator.h"
#include "AtomGoal.h"
#include "DisjunctiveSolver.h"
#include "ConjunctiveSolver.h"

using namespace std;

class Mini_SHRDLU {

private:
    int n, k, proposition;
    vector<AtomGoal> goals;

public:
    Mini_SHRDLU() {
        srand((unsigned int)time(NULL)); /// is it OK to do here?
        setup();
        selectProposition();
        play();
    }

    virtual ~Mini_SHRDLU();
    void setup();
    void selectProposition();
    void play();
    int getGoal(int flag);
};

Mini_SHRDLU::~Mini_SHRDLU() {

}

void Mini_SHRDLU::setup() {
    double choice;

    cout << "Mini SHRDLU operates on an n*n vertical board" << endl;
    cout << "Enter an integer value for 'n' such that n >= 3, n <= 30" << endl;
    cin >> choice;

    while(choice != floor(choice) ||  choice < 3 || choice > 30 || cin.fail()){
        cout << "Enter an integer value for 'n' such that n >= 3" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cin >> choice;
    }

    this->n = (int)floor(choice);
    this->k = (n*n)-n; // TODO: make k > 0, k <= (n*n) -n
}

void Mini_SHRDLU::selectProposition() {
    double choice;
    cout << "Enter 1 for Disjunctive goal, or 2 for Conjunctive goal" << endl;
    cin >> choice;

    while(choice != floor(choice) ||  choice < 1 || choice > 2 || cin.fail()){
        cout << "Enter 1 for Disjunctive goal, or 2 for Conjunctive goal" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cin >> choice;
    }
    proposition = (int)floor(choice);
}

void Mini_SHRDLU::play() {
    int block, row, col, index;
    string c; // can be char without breaking cin?
    index = 0;
    vector<AtomGoal> goals;
    StateGenerator init(k,n);
    State initialState(init.getState(),k,n);
    bool validGoal = true;

    do {
        cout << "Initial State:" << endl;
        initialState.displayState();
        cout << endl;

        cout << "Input a goal:" << endl;
        block = getGoal(0);
        row = getGoal(1);
        col = getGoal(2);

        AtomGoal a(block,row,col);

        for (int i = 0; i < goals.size(); i++) {

            if (goals[i] == a) {
                validGoal = false;
                break;
            }
        }

        if (validGoal || proposition == 1) {
            goals.push_back(a);
            cout << goals[index].getPrintedGoal() << " added." << endl;
            index++;
            cout << "Press Y to input another goal, an other key to exit." << endl;
            cin >> c;
        } else {
            validGoal = true;
            cout << "Target block already used; not solvable for conjunctive solution!" << endl;
            cout << "Input another goal:" << endl;
            c == "Y";
        }



    } while (c == "Y" || c == "y");

    if (proposition == 1) {
        cout << "Searching for Disjunctive goals:" << endl;
    } else {
        cout << "Searching for Conjunctive goals:" << endl;
    }

    /// Testing Atom_Goal Object:
    for (int i = 0; i < goals.size(); i++) {
        cout << goals[i].getPrintedGoal() << endl;
    }
    cout << endl;

    if (proposition == 1) {
        DisjunctiveSolver s(initialState.getState(), goals, k,n);
        s.solve();
    } else {
        ConjunctiveSolver s(initialState.getState(), goals, k,n);
        s.solve();
    }
}

/// Accept user input for goals
int Mini_SHRDLU::getGoal(int flag) {
    double choice;
    int min, max;
    string type;

    if (flag == 0) {
        min = 1;
        max = k;
        type = "Block";
    } else {
        min = 0;
        max = n-1;
    }

    if (flag == 1) { type = "Row"; }

    if (flag == 2) { type = "Col"; }

    cout << type << " (" << min << "-" << max << "):" << endl;
    cin >> choice;

    while(choice != floor(choice) ||  choice < min || choice > max || cin.fail()){
        cout << "Input a valid " << type << "!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cin >> choice;
    }
    return (int)floor(choice);
}

#endif //MINI_SHRDLU_V1_MINI_SHRDLU_H

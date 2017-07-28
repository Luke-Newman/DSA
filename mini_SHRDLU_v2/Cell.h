//
// Created by Luke on 28/03/2017.
//

#ifndef MINI_SHRDLU_V1_CELL_H
#define MINI_SHRDLU_V1_CELL_H

#include "AtomGoal.h"

class Cell { // This was implemented for flexibility, also comes from one of the only limitations of using a vector
                // for this assignment; I need a row and column for only a few functions
private:
    int row, col, label;

public:

    Cell(int label, int row, int col) {
        this->label = label;
        this->row = row;
        this->col = col;
    }

    void getLabel(int label) {
        this->label = label;
    }

    void setLabel(int value) {
        this-> label = value;
    }

    int getLabel() const{
        return label;
    }

    int getRow() {
        return this->row;
    }

    int getCol() {
        return this->col;
    }
};

#endif //MINI_SHRDLU_V1_CELL_H

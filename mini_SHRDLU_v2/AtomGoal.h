//
// Created by Luke on 28/03/2017.
//

#ifndef MINI_SHRDLU_V1_ATOMGOAL_H
#define MINI_SHRDLU_V1_ATOMGOAL_H

#include <iostream>
#include <string>


using namespace std;

class AtomGoal { // I chose to implement like this at the very beginning as it allowed for flexibility in finding a solution
private:
    int block, row, col;
public:
    AtomGoal(int block, int row, int col) {
        this->block = block;
        this->row = row;
        this->col = col;
    }

    int getBlock()const { return this->block; }

    int getRow()const { return this->row; }

    int getCol()const { return this->col; }

    string getPrintedGoal() const { // returns string to make cout easier

        string tmpStr;
        tmpStr += string("(") + to_string(this->block) + string(", ") + to_string(this->row) + string(", ") +
                  to_string(this->col) + string(")");
        return tmpStr;
    }

    bool operator<(const AtomGoal rhs) const{
        return this->row < rhs.getRow();
    }

    bool operator>(const AtomGoal rhs) const{
        return this->row < rhs.getRow();
    }

    // comparison operator to make sure same atom goal isn't entered twice!
    bool operator==(const AtomGoal rhs) const {
        return (this->getCol() == rhs.getCol() && this->getRow() == rhs.getRow()) || this->block == rhs.getBlock();
    }

};

#endif //MINI_SHRDLU_V1_ATOMGOAL_H

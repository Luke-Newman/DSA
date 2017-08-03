//
// Created by luken on 29/07/2017.
//

#ifndef IDEASBANK_V1_IDEA_H
#define IDEASBANK_V1_IDEA_H

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <cctype>

#include "IdeasBank.h"

using namespace std;

class Idea {
private:
    int id;
    string proposer;
    vector<string> keyWords;
    string content; // TODO: vector, map?
    vector<string> contentVector;

public:

    Idea(int id, string proposer, vector<string> keyWords, string content) {
        this->id = id;
        this->proposer = proposer;
        this->keyWords = keyWords;
        this->content = content;
        tokenize();
    }

    void printIdea() {
        cout << "id: " << id << endl;
        cout << "proposer: " << proposer << endl;

        cout << "keywords: ";
        for (int i = 0; i < keyWords.size(); i++) {
            cout << keyWords[i];

            if (i != keyWords.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;

        cout << "content: " << content << endl;

    }

    int getId() { return this->id; }

    void setID(int id) { this->id = id; }

    vector<string> getKeywords() { return this->keyWords; }

    vector<string> getContentVector() { return this->contentVector; }

    void tokenize() {
        string tmpContent = content;
        string tmpStr;

        for (int i = 0; i < content.size(); i++) {
            tmpContent[i] = trim(content[i]);
        }

        stringstream tmpStream(tmpContent);

        while (tmpStream >> tmpStr) {
            contentVector.push_back(tmpStr);
        }
    }

    char trim(char c) {
        if (iscntrl(c) || isdigit(c) || ispunct(c)) {
            return ' ';
        }

        else return (char)tolower(c);
    }

    bool keyInKeywords(const string &keyword) {
        for (string s : keyWords) {
            if (s == keyword) {
                return true;
            }
        }

        return false;
    }

    bool keyInContent(const string &keyword) {
        for (string s : contentVector) {
            if (s == keyword) {
                return true;
            }
        }

        return false;
    }

};

#endif //IDEASBANK_V1_IDEA_H

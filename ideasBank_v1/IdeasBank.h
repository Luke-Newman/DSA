//
// Created by luken on 29/07/2017.
//

#ifndef IDEASBANK_V1_IDEASBANK_H
#define IDEASBANK_V1_IDEASBANK_H

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <limits>
#include <cmath>
#include <set>
#include <string>

#include "Idea.h"
#include "FileHandler.h"
#include "AVL_Tree.h"

using namespace std;

struct KEYWORD {
    string key;
    vector<int> idList;
};

void printNode(KEYWORD k) {
    cout << k.key << " { ";
    for (int i : k.idList) {
        cout << i << " ";
    }
    cout << "}\n" << endl;
}

class IdeasBank {
private:

    FileHandler *reader;
    map<int, Idea> ideasMap;
    AvlTree<KEYWORD, string> *ideasAVL;

public:

    IdeasBank() {
        reader = new FileHandler();
        ideasAVL = new AvlTree<KEYWORD, string>();
    }

    void getData();
    void insertAVL(vector<Idea> ideaVect);
    void printIdeasBank();
    void run();
    void menu();
    void getIdea();
    vector<string> getKeyWords();
    string getContent();
    void searchIdeasBank();
    void searchIdeasAVL();
    void searchRelatedIdeas();
    void insertNewIdeaAVL(Idea i);
    void deleteIdea();

    virtual ~IdeasBank();
};

IdeasBank::~IdeasBank() {
    cout << "Program terminated; deleting file reader and AVL tree" << endl;
    delete reader;
    delete ideasAVL; // Deletes pointer to class (AVL destructor should delete all child nodes in tree)
}

void IdeasBank::getData() {
    fstream inFile;
    string path;
    vector<Idea> tmpList;
    int idNo;

    cin.clear();
    cout << "Enter full path of file" << endl;
    cin >> path;

    if (cin.fail()) {
        cout << "cin fail" << endl;
        return;
    }

    inFile.open(path);

    if (!inFile.is_open()) {
        cout << "Could not open ideas file!" << endl;
        return;
    }

    if (ideasMap.empty()) {
        tmpList = reader->getIdeas(inFile, 1);
    } else {
        tmpList = reader->getIdeas(inFile, ideasMap.rbegin()->first+1);
    }

    for (Idea i : tmpList) {
        idNo = i.getId();
        ideasMap.insert(std::pair<int, Idea>(idNo, i));
    }

    insertAVL(tmpList);

}

void IdeasBank::insertAVL(vector<Idea> ideaVect) {
    for (Idea i : ideaVect) {
        vector<string> keywords = i.getKeywords();
        vector<string> content = i.getContentVector();

        for (string s : keywords) { // TODO add content after testing
            KEYWORD k;

            k.key = s; // TODO: need to check if key exists and increment in AVL tree function
            k.idList.push_back(i.getId());
            ideasAVL->AVL_Insert(k);

        }

        for (string s : content) {
            KEYWORD k;

            k.key = s; // TODO: need to check if key exists and increment in AVL tree function
            k.idList.push_back(i.getId());
            ideasAVL->AVL_Insert(k);
        }
    }
}

void IdeasBank::printIdeasBank() {

    if (ideasMap.empty()) {
        cout << "Ideas Bank is empty! Enter an idea or import from a file." << endl;
        return;
    }

    for (auto &it : ideasMap) {
        it.second.printIdea();
        cout << endl;
    }
}

void IdeasBank::run() {
    menu();
    // TODO: does the text file need to be written to?
    cout << "Exiting" << endl;
}

void IdeasBank::menu() {
    bool running = true;
    int choice;
    string tmpPath;

    while(running) {
        cout << "\nmain menu\n 1: import ideas from a file\n 2: print ideas bank\n 3: enter a new idea\n"
                " 4: print word indices\n 5: search for related ideas in ideas bank\n"
                " 6: search for related ideas in indexing tree\n"
                " 7: Search for idea ID's related to an idea\n 8: delete idea\n 9: print AVL tree\n 0: exit" << endl;
        cin >> choice;

        while (choice < 0 || choice > 9 || cin.fail()) {
            cout << "\nmain menu\n 1: import ideas from a file\n 2: print ideas bank\n 3: enter a new idea\n"
                    " 4: print word indices\n 5: search for related ideas in ideas bank\n"
                    " 6: search for related ideas in indexing tree\n"
                    " 7: Search for idea ID's related to an idea\n 8: delete idea\n 9: print AVL tree\n 0: exit" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cin >> choice;
        }

        switch (choice) {
            case 0 :    running = false;
                        break;

            case 1:     getData();
                        break;

            case 2:     printIdeasBank();
                        break;

            case 3 :    getIdea();
                        break;

            case 4:     ideasAVL->AVL_Traverse(printNode);
                        break;

            case 5 :    searchIdeasBank();
                        break;

            case 6:     searchIdeasAVL();
                        break;

            case 7:     searchRelatedIdeas();
                        break;

            case 8:     deleteIdea();
                        break;

            case 9:     cout << "Printed AVL tree:\n" << endl;
                        ideasAVL->AVL_Print();
                        break;

            default :   cout << "Wrong input" << endl;
                        break;
        }
    }
}

void IdeasBank::getIdea() {
    string proposer;
    vector<string> keyWords;
    string content; // TODO: make map??
    KEYWORD tmpKeyword;
    int id;

    if (ideasAVL->AVL_Empty()) {
        cout << "Ideas Bank is empty! Enter an idea or import from a file." << endl;
        return;
    }

    cout << "enter the name of the proposer" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, proposer);

    id = ideasMap.rbegin()->first+1;
    keyWords = getKeyWords();
    content = getContent();

    ideasMap.insert(std::pair<int, Idea>(id, Idea(id, proposer, keyWords, content)));
    //ideasMap.push_back(Idea(id, proposer, keyWords, content));

    for (string key : keyWords) {
        tmpKeyword.key = key;
        tmpKeyword.idList.push_back(id);
        ideasAVL->AVL_Insert(tmpKeyword);
    }

    for (string key : ideasMap.at(id).getContentVector()) {
        tmpKeyword.key = key;
        tmpKeyword.idList.push_back(id);
        ideasAVL->AVL_Insert(tmpKeyword);
    }

    cout << "received input for new idea\n" << endl;
    ideasMap.at(id).printIdea();
    insertNewIdeaAVL(ideasMap.at(id));
}

vector<string> IdeasBank::getKeyWords() {
    vector<string> words;
    string tmpStr;
    bool running = true;

    cout << "Type a key word followed by enter for each key, 0 to finish" << endl;
    while(running) {
        cin >> tmpStr;
        if (tmpStr == "0")
            break;

        words.push_back(tmpStr);
    }

    cout << "   Entered keywords:" << endl;
    for (int i = 0; i < words.size(); i++) {
        cout << words[i] << endl;
    }

    return words;
}

string IdeasBank::getContent() {
    string text;
    string line; // used as buffer

    cout << "Type the content of your idea:\n";
    // need to flush cin because entered int value 0 before to exit keywords loop
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    do {
        getline(cin,line);
        text += line + "\n";

    } while(line != "");

    // debugging
    cout << "Your content:\n" << text << endl;

    return text;
}

void IdeasBank::searchIdeasBank() {
    bool match = false;
    map<int, Idea>::iterator it;
    string key;

    if (ideasMap.empty()) {
        cout << "Ideas Bank is empty! Enter an idea or import from a file." << endl;
        return;
    }

    cout << "\nEnter a word" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // test to clear cin stream
    cin >> key;

    cout << endl;
    cout << "Search results from ideas bank:\n" << endl;
    for (it = ideasMap.begin(); it != ideasMap.end(); it++) {
        if (it->second.keyInKeywords(key) || it->second.keyInContent(key)) {
            match = true;
            it->second.printIdea();
            cout << endl;
        }
    }

    if (!match) {
        cout << "-- No match found in ideas bank --\n" << endl;
    }
}

void IdeasBank::searchIdeasAVL() {
    string key;

    if (ideasAVL->AVL_Empty()) {
        cout << "Ideas Bank is empty! Enter an idea or import from a file." << endl;
        return;
    }

    cout << "\nEnter a word" << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // test to clear cin stream
    cin >> key;

    cout << endl;
    cout << "Search results from AVL tree:\n" << endl;
    KEYWORD k;
    ideasAVL->AVL_Retrieve(key, k);

    if (k.idList.empty()) {
        cout << "-- No match found in AVL tree --\n" << endl;
    } else {
        for (int i = 0; i < k.idList.size(); i++) {
            ideasMap.at(k.idList[i]).printIdea();
            cout << endl;
        }
    }
    cout << endl;
}

void IdeasBank::searchRelatedIdeas() {
    int id;
    vector<string> keywords;
    vector<Idea> relatedIdeas;
    KEYWORD k;
    vector<int> idList, tmpList;

    if (ideasAVL->AVL_Empty()) {
        cout << "Ideas Bank is empty! Enter an idea or import from a file." << endl;
        return;
    }

    cout << "enter the id no. to search for related ideas" << endl;
    cin >> id;

    while (id < ideasMap.begin()->first || id > ideasMap.rbegin()->first || cin.fail()) {
        cout << "enter the id no. to search for related ideas" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cin >> id;
    }

    keywords = ideasMap.at(id).getKeywords();

    for (string key : keywords) {
        ideasAVL->AVL_Retrieve(key, k);
        tmpList = k.idList;

        idList.insert(idList.end(), tmpList.begin(), tmpList.end());
    }

    set<int> tmpSet(idList.begin(),idList.end()); // apparently default constructor is faster than manual
    tmpSet.erase(id); // removal based on "key" (int value)
    idList.assign(tmpSet.begin(),tmpSet.end());

    if (!idList.empty()) {
        cout << "\nIdeas related to idea ID " << id << ":\n" << endl;

        for (int i : idList) {
            ideasMap.at(i).printIdea();
            cout << endl;
        }
    } else {
        cout << " -- No ideas with matching keywords found --\n" << endl;
    }
}

void IdeasBank::insertNewIdeaAVL(Idea i) {
    vector<string> keywords = i.getKeywords();
    vector<string> content = i.getContentVector();

    for (string s : keywords) { // TODO add content after testing
        KEYWORD k;
        k.key = s; // TODO: need to check if key exists and increment in AVL tree function
        k.idList.push_back(i.getId());
        ideasAVL->AVL_Insert(k);
    }

    for (string s : content) {
        KEYWORD k;
        k.key = s; // TODO: need to check if key exists and increment in AVL tree function
        k.idList.push_back(i.getId());
        ideasAVL->AVL_Insert(k);
    }

}

void IdeasBank::deleteIdea() {
    vector<string> keywordsToDelete;
    int id;

    if (ideasMap.empty()) {
        cout << "Ideas Bank is empty! Enter an idea or import from a file." << endl;
        return;
    }

    cout << "enter the id no. of idea you wish to delete" << endl;
    cin >> id;

    while (id < 1 || id > ideasMap.rbegin()->first || cin.fail()) {
        cout << "enter the id no. of idea you wish to delete" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cin >> id;
    }

    keywordsToDelete = ideasAVL->traverseMarkForRemoval(id);

    cout << "The following words have no ID left and are marked for removal:" << endl;
    for (string s : keywordsToDelete) {
        cout << s << " ";
    }
    cout << "\n" << endl;

    for (string s : keywordsToDelete) { // removing words that now have no ID
        ideasAVL->AVL_Delete(s);
    }

    ideasMap.erase(id);
}

#endif //IDEASBANK_V1_IDEASBANK_H

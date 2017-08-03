//
// Created by luken on 29/07/2017.
//

#ifndef IDEASBANK_V1_FILEHANDLER_H
#define IDEASBANK_V1_FILEHANDLER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <locale>
#include <string>

#include "Idea.h"

using namespace std;

class FileHandler {

private:

public:

    FileHandler() = default;

    vector<Idea> getIdeas(fstream &inFile, int id) {
        vector<Idea> ideasVector;
        int tmpID;
        vector<string> tmpKeywords;
        string tmpProposer, tmpContent ,tmpStr, tmpData;

        int it = id;
        int testCount = 1;

        while(!inFile.eof()) {

            getline(inFile, tmpStr);

            tmpData += tmpStr;

            if (tmpStr == "#end#") {
                ideasVector.push_back(processIdea(tmpData, it));
                tmpData = "";
                it++;
            }
        }

        return ideasVector;
    }

    Idea processIdea(string tmpIdea, int idNo) {

        string tmpProposer, tmpContent;
        vector<string> tmpKeyWords;
        size_t begin, end;

        // getting Proposer name
        begin = getRightOfDelim(tmpIdea, "#proposer#");
        end = tmpIdea.find("#keywords#");
        tmpProposer = getString(tmpIdea, begin, end);

        begin = getRightOfDelim(tmpIdea, "#keywords#");
        end = tmpIdea.find("#content#");
        tmpKeyWords = getKeywords(tmpIdea, begin, end);

        begin = getRightOfDelim(tmpIdea, "#content#");
        end = tmpIdea.find("#end#");
        tmpContent = getString(tmpIdea, begin, end);

        return Idea(idNo, tmpProposer, tmpKeyWords, tmpContent);
    }


    size_t getRightOfDelim(string const& str, string const& delim) {
        // I wanted to find end of tag and also make the function multipurpose
        return (str.find(delim) + delim.size());
    }

    string getString(string str, size_t begin, size_t end) {
        string tmpStr;

        for (size_t i = begin; i < end; i++) {

            tmpStr += str[i];

        }
        tmpStr = ltrim(tmpStr);
        tmpStr = rtrim(tmpStr);
        return tmpStr;
    }

    vector<string> getKeywords(string str, size_t begin, size_t end) {
        string substring = str.substr(begin, end-begin);
        stringstream tmpStream(substring);
        string tmpStr;
        vector<string> keywordsVector;

        while (tmpStream >> tmpStr) {
            keywordsVector.push_back(tmpStr);
        }

        return keywordsVector;
    }

    /* These two functions are used to trim unnecessary white space left over on text file,
       using lambda/anonymous function */
    string & ltrim(string & str) {
        auto it2 =  find_if(str.begin(), str.end() ,[](char ch){ return !std::isspace<char>(ch, std::locale::classic()); } );
        str.erase( str.begin(), it2);
        return str;
    }

    string & rtrim(std::string & str) {
        auto it1 =  std::find_if(str.rbegin(), str.rend(), [](char ch){ return !std::isspace<char>(ch, std::locale::classic()); } );
        str.erase( it1.base(), str.end() );
        return str;
    }

};

#endif //IDEASBANK_V1_FILEHANDLER_H

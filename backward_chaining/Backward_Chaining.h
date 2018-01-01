//
// Created by luken on 7/10/2017.
//

#ifndef BACKWARD_CHAINING_V2_BACKWARD_CHAINING_H
#define BACKWARD_CHAINING_V2_BACKWARD_CHAINING_H

#include <utility>
#include <vector>
#include <fstream>
#include <limits>
#include <iostream>
#include <sstream>
#include <list>
#include <unordered_map>

/** CPP 14 **/

class Backward_Chaining
{
private:
    /* Knowledge base into 2 components
     * (propositions and facts), which allows mostly for a faster lookup
     * time, but also allowed for storing of propositions to be more compact. */

    /*  - key, value pair for map is head, then vector of propositions
     *  - first value in vector pair is the connective, second value is a
     *    string containing the associated truth values */
    std::unordered_map<char, std::vector<std::pair<char, std::string> > > knowledge_base;

    /* The value in this map is redundant, I only want the O(1) access */
    std::unordered_map<char, bool> data_base;

    const char AND = '^';
    const char OR = 'v';

    void print_KB();
    void print_DB();
    void build_KB();
    bool valid_proposition(std::string &input, char &connective);
    void process_proposition(std::string prop, char connective);
    char query_KB();
    void search_KB(char query);
    bool satisfied_AND(std::list<std::pair<char, bool> > &query_stack,
                       std::string body, bool subject_visited);
    bool satisfied_OR(std::list<std::pair<char, bool> > &query_stack,
                      std::string body, bool subject_visited);

public:
    Backward_Chaining() = default;
    void run();
    ~Backward_Chaining() = default;
};

void Backward_Chaining::run()
{
    char query;
    build_KB();

    while (true)
    {
        query = query_KB();

        if (isdigit(query))
            break;

        search_KB(toupper(query));
    }
}

void Backward_Chaining::print_KB()
{
    std::string connective;

    for (const auto &it : knowledge_base)
    {
        for (auto i : it.second)
        {
            if (i.first == AND)
                connective = "(AND)";
            else if (i.first == OR)
                connective = "(OR)";
            else
                connective = "";

            std::cout << i.second << "=>" << it.first << " " << connective << std::endl;
        }
    }
}

void Backward_Chaining::print_DB()
{
    for (const auto &it : data_base)
        std::cout << it.first << std::endl;
}

void Backward_Chaining::build_KB()
{
    std::string text;
    std::string line;
    char connective;

    std::cout<< "\nThe Knowledge Base can only accept facts"
            " in the format:\nP1^P2 ... ^Pk=>P, or\nP1vP2 ."
            ".. vPk=>P, or\nP\nPlease input your "
            "knowledge base, press enter twice to finish.\n";

    do {
        getline(std::cin,line);

        if (!valid_proposition(line, connective))
        {
            if (line.empty())
                break;

            std::cout<< "The Knowledge Base can only accept facts"
                    " in the format:\nP1^P2 ...^Pk=>P, or\nP1vP2 ."
                    ".. vPk=>P, or\nP\nPlease input your "
                    "knowledge base, press enter twice to finish.\n";
        }
        else
        {
            std::cout << "Input Accepted\n";
            process_proposition(line, connective);
            connective = '\0';
        }
        std::cin.clear();

    } while(!line.empty());

    std::cout << "Knowledge base:\n";
    print_KB();
    std::cout << "\nDatabase:\n";
    print_DB();
}

bool Backward_Chaining::valid_proposition(std::string &input, char &connective)
{
    int flag = false;

    std::istringstream iss(input);
    std::string token;

    if (input.size() == 1)
    {
        if (isalpha(input[0]))
        {
            connective = '\0';
            return true;
        }
    }

    for (char c : input) {
        if (c == AND || c == OR)
            connective = c;

        if (isdigit(c))
            return false;
    }

    if (input.size() != 4 && connective == '\0')
        return false;

    while (std::getline(iss, token, connective))
    {
        if (token.length() > 1)
        {
            if (flag)
                return false;

            if (token.find("=>") != std::string::npos)
                flag = true;
            else
                return false;

        }
        else if (token.length() == 0)
            return false;

        if (!isalpha(token[0]))
            return false;
    }

    return !(token.length() != 4 || !flag);
}

/* A parser that processes a valid proposition into KB or DB */
void Backward_Chaining::process_proposition(std::string prop, char connective)
{
    char head, tmp_body;
    std::string body;

    /* It is a fact */
    if (prop.size() == 1)
    {
        head = (toupper(prop[0]));
        auto search = data_base.find(head);

        if (search == data_base.end())
            data_base.insert(std::make_pair(head, false));

        return;
    }

    for (char c : prop) {
        if (c == '=')
        {
            break;
        }

        if (c == AND || c == OR)
        {
            continue;
        }

        tmp_body = (toupper(c));
        body += tmp_body;
    }

    head = toupper(prop[prop.size()-1]);
    auto search = knowledge_base.find(head);

    if (search == knowledge_base.end())
    {
        std::vector<std::pair<char, std::string> > tmp;
        tmp.emplace_back(std::make_pair(connective, body));
        knowledge_base.insert(std::make_pair(head, tmp));
    }
    else
        search->second.emplace_back(connective, body);

}

char Backward_Chaining::query_KB()
{
    char query;

    do
    {
        std::cin.clear();
        std::cout << "\nEnter query (single alpha character), digit to quit.\n";
        std::cin >> query;
    }
    while (std::cin.fail());

    return query;
}

/* Essentially a depth-first search, the linked list allows us to keep track of
 * the parent member (subject); It can be deleted in O(1) and it's location will
 * not be invalidated regardless of how many children are added to the list */
void Backward_Chaining::search_KB(char query)
{
    char subject;

    /* char is head, bool is to check whether head has been processed before */
    std::list<std::pair<char, bool> >::iterator subject_loc;

    /* stores bodies of propositions associated with a particular head
     * char is connective. string is body, */
    std::vector<std::pair<char, std::string> > instances_of_subject;

    std::list<std::pair<char, bool> > query_stack;
    bool subject_solved = false, subject_visited;

    query_stack.push_front(std::make_pair(query, false));

    while (!query_stack.empty())
    {
        subject_loc = query_stack.begin();
        subject = subject_loc->first;
        subject_visited = subject_loc->second;

        std::cout<< "Subject: " << subject << std::endl;

        auto search = knowledge_base.find(subject);

        if (search == knowledge_base.end())
        {
            query_stack.pop_front();
            continue;
        }

        instances_of_subject = search->second;

        for (auto s : instances_of_subject)
        {
            if (s.first == AND)
            {
                if (satisfied_AND(query_stack, s.second, subject_visited))
                {
                    data_base.insert(std::make_pair(subject, true));
                    subject_solved = true;
                }
            }
            else
            {
                if (satisfied_OR(query_stack, s.second, subject_visited))
                {
                    data_base.insert(std::make_pair(subject, true));
                    subject_solved = true;
                }
            }

        }

        if (subject_solved)
        {
            subject_solved = false;
            query_stack.erase(subject_loc);
            continue;
        }

        /* subject (head) visited twice to check if children (body) were true
         * the second time we visit we only want to check if it's now satisfied*/
        if (subject_visited)
            query_stack.erase(subject_loc);
        else
            subject_loc->second = true;

    }

    if (data_base.find(query) != data_base.end())
        std::cout << "True\n";
    else
        std::cout << "False\n";
}

bool Backward_Chaining::satisfied_AND(std::list<std::pair<char, bool> > &query_stack,
                                      std::string body, bool subject_visited)
{
    bool solved = true;

    for (char c : body)
    {
        auto search = data_base.find(c);

        if (search == data_base.end())
        {
            if (!subject_visited)
                query_stack.push_front(std::make_pair(c, false));

            solved = false;
        }
    }

    return solved;
}

bool Backward_Chaining::satisfied_OR(std::list<std::pair<char, bool> > &query_stack,
                                     std::string body, bool subject_visited)
{
    bool solved = false;

    for (char c : body)
    {
        auto search = data_base.find(c);

        if (search != data_base.end())
            solved = true;
        else if (!subject_visited)
            query_stack.push_front(std::make_pair(c, false));
        /* The second round of search is only to check if searching children
         * in the body made the parent true, we do not need to push in again */
    }

    return solved;
}

#endif //BACKWARD_CHAINING_V2_BACKWARD_CHAINING_H

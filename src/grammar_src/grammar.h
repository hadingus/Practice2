#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <iostream>
#include <vector>
#include <string>

class Grammar {
    int size_;
    std::vector<std::string> rules_;
public:
    static bool isValidSingleRule(const std::string &rule);
    static bool isValidRule(const std::string &rule);
    static bool isNonTerminal(char c);
    static bool isSymbol(char c);

    Grammar(int size = 0);

};

#endif

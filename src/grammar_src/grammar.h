#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <iostream>
#include <vector>
#include <string>
#include <set>

class RuleVerifier {
public:
    static std::vector<std::string> parseRules(const std::string &rule);
    static bool isValidSingleRule(const std::string &rule);
    static bool isValidRule(const std::string &rule);
    static bool isNonTerminal(char c);
    static bool isSymbol(char c);
};

class Grammar {
    char start_;
    std::vector<std::string> rules_;
public:
    Grammar(char start = 'S');
    Grammar(const Grammar &other);
    Grammar(Grammar &&other);
    Grammar& operator=(const Grammar &other);
    Grammar& operator=(Grammar &&other);

    char getStart() const;
    bool addRule(const std::string &rule);
    unsigned int size() const;
    void deleteSimilarRules();
    std::string& operator[](int id);
    const std::string& operator[](int id) const;
};

#endif

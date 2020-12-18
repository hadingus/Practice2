#ifndef EARLEY_ALGO_H
#define EARLEY_ALGO_H

#include <iostream>
#include "grammar_src/grammar.h"

class EarleyAlgo {
    struct state {
        std::string rule;
        int rulePos;
        int strPos;

        state(const std::string &rule, int rulePos, int strPos);
        bool operator==(const state &other) const;
        bool operator!=(const state &other) const;
        bool operator<(const state &other) const;
    };

    std::vector<std::set<state>> levels;
    Grammar grammar_;

    using stateIterator = std::set<state>::iterator;

    bool scan(stateIterator it, int levelId, char c);
    bool predict(stateIterator it, int levelId);
    bool complete(stateIterator it, int levelId);

    void scan(int id, const std::string &s);
    bool predict(int id);
    bool complete(int id);
public:
    EarleyAlgo();
    EarleyAlgo(Grammar grammar);
    bool hasWord(const std::string &word);
};

#endif

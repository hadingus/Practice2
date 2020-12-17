#include "grammar.h"

Grammar::Grammar(int size): size_(size) {}

bool Grammar::isNonTerminal(char c) {
    return c >= 'A' && c <= 'Z';
}

bool Grammar::isSymbol(char c) {
    return c >= 'a' && c <= 'z';
}

bool Grammar::isValidSingleRule(const std::string &rule) {
    bool good = rule.size() > 3 && isNonTerminal(rule[0]) && rule[1] == '-' && rule[2] == '>';
    if (good && rule.size() == 4 && rule.back() == '1') {
        return true;
    }
    for (int i = 3; good && i < rule.size(); ++i) {
        good = isSymbol(rule[i]) || isNonTerminal(rule[i]);
    }
    return good;
}

bool Grammar::isValidRule(const std::string &rule) {
    bool good = rule.size() > 3 && isNonTerminal(rule[0]) && rule[1] == '-' && rule[2] == '>';
    std::string start = rule.substr(0, 3);
    std::string curRule = start;
    for (int i = 3; i < rule.size(); ++i) {
        if (rule[i] == '|') {
            if (!isValidSingleRule(curRule)) {
                return false;
            } else {
                curRule = start;
            }
        } else {
            curRule += rule[i];
        }
    }
    return isValidSingleRule(curRule);
}
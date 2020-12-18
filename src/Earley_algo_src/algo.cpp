#include "algo.h"

EarleyAlgo::EarleyAlgo(Grammar grammar): grammar_(grammar) {}
EarleyAlgo::EarleyAlgo() {}

EarleyAlgo::state::state(const std::string &rule, int rulePos, int strPos): rule(rule), rulePos(rulePos), strPos(strPos) {}
bool EarleyAlgo::state::operator==(const state &other) const {
    return rule == other.rule && rulePos == other.rulePos && strPos == other.strPos;
}

bool EarleyAlgo::state::operator!=(const state &other) const {
    return !(*this == other);
}

bool EarleyAlgo::state::operator<(const state &other) const {
    if (rulePos != other.rulePos) {
        return rulePos < other.rulePos;
    }
    if (strPos != other.strPos) {
        return strPos < other.strPos;
    }
    return rule < other.rule;
}

bool EarleyAlgo::scan(stateIterator it, int levelId, char c) {
    if (isSymbol(c) && it->rule[it->rulePos] == c) {
        int prev_sz = levels[levelId + 1].size();
        levels[levelId + 1].insert(state(it->rule, it->rulePos + 1, it->strPos));
        return levels[levelId + 1].size() != prev_sz;
    }
    return false;
}

bool EarleyAlgo::predict(stateIterator it, int levelId) {
    if (it->rulePos < it->rule.size() && isNonTerminal(it->rule[it->rulePos])) {
        char nonTerm = it->rule[it->rulePos];
        std::vector<state> new_states;
        for (auto nxt_it = grammar_.begin(nonTerm); nxt_it != grammar_.end(nonTerm); ++nxt_it) {
            new_states.emplace_back(*nxt_it, 3, levelId);
        }
        int prev_sz = levels[levelId].size();
        for (const auto& state : new_states) {
            levels[levelId].insert(state);
        }
        return levels[levelId].size() != prev_sz;
    }
    return false;
}

bool EarleyAlgo::complete(stateIterator it, int levelId) {
    if (it->rulePos == it->rule.size()) {
        char nonTerminal = it->rule[0];
        int lvl = it->strPos;
        std::vector<state> new_states;
        for (stateIterator prev_it = levels[lvl].begin(); prev_it != levels[lvl].end(); ++prev_it) {
            if (prev_it->rulePos < prev_it->rule.size() && prev_it->rule[prev_it->rulePos] == nonTerminal) {
                new_states.emplace_back(prev_it->rule, prev_it->rulePos + 1, prev_it->strPos);
            }
        }
        int prev_sz = levels[levelId].size();
        for (const auto & new_state : new_states) {
            levels[levelId].insert(new_state);
        }
        return levels[levelId].size() != prev_sz;
    }
    return false;
}

void EarleyAlgo::scan(int id, const std::string &s) {
    for (auto it = levels[id].begin(); it != levels[id].end(); ++it) {
        scan(it, id, s[id]);
    }
}

bool EarleyAlgo::predict(int id) {
    bool changed = false;
    std::vector<stateIterator> its;
    for (auto it = levels[id].begin(); it != levels[id].end(); ++it) {
        its.push_back(it);
    }
    for (auto it : its) {
        changed |= predict(it, id);
    }
    return changed;
}

bool EarleyAlgo::complete(int id) {
    bool changed = false;
    std::vector<stateIterator> its;
    for (auto it = levels[id].begin(); it != levels[id].end(); ++it) {
        its.push_back(it);
    }
    for (auto it : its) {
        changed |= complete(it, id);
    }
    return changed;
}

bool EarleyAlgo::hasWord(const std::string &word) {
    levels.assign(word.size() + 1, std::set<state>());
    std::string startRule = "#->";
    startRule += grammar_.getStart();
    levels[0].insert(state(startRule, 3, 0));
    bool changed = true;
    while (changed) {
        changed = complete(0);
        changed |= predict(0);
    }

    for (int id = 0; id < word.size(); ++id) {
        scan(id, word);
        changed = true;
        while (changed) {
            changed = complete(id + 1);
            changed |= predict(id + 1);
        }
    }
    state result(startRule, 4, 0);
    for (auto state : levels[word.size()]) {
        if (state == result) {
            return true;
        }
    }
    return false;
}
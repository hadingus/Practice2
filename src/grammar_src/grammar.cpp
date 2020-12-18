#include "grammar.h"

Grammar::Grammar(char start): start_(start) {}

bool RuleVerifier::isNonTerminal(char c) {
    return c >= 'A' && c <= 'Z';
}

bool RuleVerifier::isSymbol(char c) {
    return c >= 'a' && c <= 'z';
}

std::vector<std::string> RuleVerifier::parseRules(const std::string &rule) {
    std::string start = rule.substr(0, 3);
    std::vector<std::string> rulesArr;
    std::string curRule = start;
    for (int i = 3; i < rule.size(); ++i) {
        if (rule[i] == '|') {
            rulesArr.push_back(curRule);
            curRule = start;
        } else {
            curRule += rule[i];
        }
    }
    rulesArr.push_back(curRule);
    return rulesArr;
}

bool RuleVerifier::isValidSingleRule(const std::string &rule) {
    bool good = rule.size() > 3 && isNonTerminal(rule[0]) && rule[1] == '-' && rule[2] == '>';
    if (good && rule.size() == 4 && rule.back() == '1') {
        return true;
    }
    for (int i = 3; good && i < rule.size(); ++i) {
        good = isSymbol(rule[i]) || isNonTerminal(rule[i]);
    }
    return good;
}

bool RuleVerifier::isValidRule(const std::string &rule) {
    bool good = rule.size() > 3 && isNonTerminal(rule[0]) && rule[1] == '-' && rule[2] == '>';
    if (!good) {
        return false;
    }
    auto singRules = parseRules(rule);
    for (const auto& sRule : singRules) {
        if (!isValidSingleRule(sRule)) {
            return false;
        }
    }
    return true;
}

bool Grammar::addRule(const std::string &rule) {
    if (RuleVerifier::isValidRule(rule)) {
        auto rulesPack = RuleVerifier::parseRules(rule);
        for (const auto& sRule : rulesPack) {
            rules_.push_back(sRule);
        }
        return true;
    }
    return false;
}

unsigned int Grammar::size() const {
    return rules_.size();
}

void Grammar::deleteSimilarRules() {
    std::set<std::string> single_rules;
    for (const auto& rule : rules_) {
        single_rules.insert(rule);
    }
    rules_.clear();
    for (const auto& rule : single_rules) {
        rules_.push_back(rule);
    }
}

std::string& Grammar::operator[](int id) {
    return rules_[id];
}

const std::string & Grammar::operator[](int id) const {
    return rules_[id];
}

Grammar::Grammar(const Grammar &other): rules_(other.rules_), start_(other.start_) {}
Grammar::Grammar(Grammar &&other): rules_(std::move(other.rules_)), start_(other.start_) {}

Grammar & Grammar::operator=(Grammar &&other) {
    start_ = other.start_;
    rules_ = std::move(other.rules_);
    return *this;
}

Grammar& Grammar::operator=(const Grammar &other) {
    start_ = other.start_;
    rules_ = other.rules_;
    return *this;
}

char Grammar::getStart() const {
    return start_;
}

std::ostream& operator <<(std::ostream &stream, const Grammar &g) {
    int sz = g.size();
    stream << sz << "\n";
    for (int i = 0; i < sz; ++i) {
        if (i + 1 != sz) {
            stream << g[i] << "\n";
        } else {
            stream << g[i];
        }
    }
    return stream;
}

std::istream& operator >>(std::istream& stream, Grammar &g) {
    int sz;
    stream >> sz;
    std::string rule;
    for (int i = 0; i < sz; ++i) {
        stream >> rule;
        g.addRule(rule);
    }
    return stream;
}
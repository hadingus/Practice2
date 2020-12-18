#include "grammar.h"

Grammar::Grammar(char start): start_(start), rules_(maxCharId), size_(0) {}

bool isNonTerminal(char c) {
    return c >= 'A' && c <= 'Z';
}

bool isSymbol(char c) {
    return c >= 'a' && c <= 'z';
}

std::vector<std::string> parseRules(const std::string &rule) {
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

bool isValidSingleRule(const std::string &rule) {
    bool good = rule.size() > 3 && isNonTerminal(rule[0]) && rule[1] == '-' && rule[2] == '>';
    if (good && rule.size() == 4 && rule.back() == '1') {
        return true;
    }
    for (int i = 3; good && i < rule.size(); ++i) {
        good = isSymbol(rule[i]) || isNonTerminal(rule[i]);
    }
    return good;
}

bool isValidRule(const std::string &rule) {
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
    if (isValidRule(rule)) {
        auto rulesPack = parseRules(rule);
        for (const auto& sRule : rulesPack) {
            rules_[sRule[0] - 'A'].push_back(sRule);
            ++size_;
        }
        return true;
    }
    return false;
}

unsigned int Grammar::size() const {
    return size_;
}

void Grammar::deleteSimilarRules() {
    size_ = 0;
    std::set<std::string> single_rules;
    for (const auto& rule : *this) {
        single_rules.insert(rule);
    }
    for (int i = 0; i < maxCharId; ++i) {
        rules_[i].clear();
    }
    for (const auto& rule : single_rules) {
        rules_[rule[0] - 'A'].push_back(rule);
        ++size_;
    }
}

Grammar::Grammar(const Grammar &other): rules_(other.rules_), start_(other.start_), size_(other.size_) {}
Grammar::Grammar(Grammar &&other): rules_(std::move(other.rules_)), start_(other.start_), size_(other.size_) {}

Grammar & Grammar::operator=(Grammar &&other) {
    start_ = other.start_;
    size_ = other.size_;
    rules_ = std::move(other.rules_);
    return *this;
}

Grammar& Grammar::operator=(const Grammar &other) {
    start_ = other.start_;
    size_ = other.size_;
    rules_ = other.rules_;
    return *this;
}

char Grammar::getStart() const {
    return start_;
}

std::ostream& operator <<(std::ostream &stream, const Grammar &g) {
    int sz = g.size();
    stream << sz << "\n";
    auto it = g.begin();
    for (int i = 0; i < sz; ++i) {
        if (i + 1 != sz) {
            stream << *it << "\n";
        } else {
            stream << *it;
        }
        ++it;
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

template<bool isConst>
bool Grammar::Iterator_<isConst>::isValid() const {
    return ruleId_ >= 0 && ruleId_ < (*ptr_)[charId_].size();
}

template<bool isConst>
Grammar::Iterator_<isConst>::Iterator_(ptrType ptr, int charId, int ruleId): charId_(charId),
                                                                                        ruleId_(ruleId), ptr_(ptr) {}

template<bool isConst>
Grammar::Iterator_<isConst>::Iterator_(const Iterator_ &other): charId_(other.charId_), ruleId_(other.ruleId_),
                                                                                        ptr_(other.ptr_) {}

template<bool isConst>
Grammar::Iterator_<isConst> & Grammar::Iterator_<isConst>::operator=(const Iterator_ &other) {
    ruleId_ = other.ruleId_;
    ptr_ = other.ptr_;
    charId_ = other.charId_;
}

template<bool isConst>
Grammar::Iterator_<isConst>& Grammar::Iterator_<isConst>::operator++() {
    ruleId_++;
    if (ruleId_ >= (*ptr_)[charId_].size()) {
        ruleId_ = 0;
        ++charId_;
        while (charId_ < maxCharId && (*ptr_)[charId_].empty()) {
            ++charId_;
        }
    }
    return *this;
}

template<bool isConst>
Grammar::Iterator_<isConst>& Grammar::Iterator_<isConst>::operator--() {
    if (ruleId_ <= 0) {
        --charId_;
        while (charId_ >= 0 && (*ptr_)[charId_].empty()) {
            --charId_;
        }
        if (charId_ >= 0) {
            ruleId_ = (*ptr_)[charId_].size();
        }
    } else {
        ruleId_ = 0;
    }
    --ruleId_;
    return *this;
}

template<bool isConst>
typename Grammar::Iterator_<isConst>::reference Grammar::Iterator_<isConst>::operator*() {
    return (*ptr_)[charId_][ruleId_];
}

template<bool isConst>
typename Grammar::Iterator_<isConst>::pointer Grammar::Iterator_<isConst>::operator->() {
    return &((*ptr_)[charId_][ruleId_]);
}

template<bool isConst>
bool Grammar::Iterator_<isConst>::operator==(const Iterator_ &other) const {
    return ptr_ == other.ptr_ && charId_ == other.charId_ && ruleId_ == other.ruleId_;
}

template<bool isConst>
bool Grammar::Iterator_<isConst>::operator!=(const Iterator_ &other) const {
    return !(*this == other);
}

Grammar::Iterator Grammar::begin() {
    Iterator res = Iterator(&rules_, 0, 0);
    if (!res.isValid()) {
        ++res;
    }
    return res;
}

Grammar::Iterator Grammar::end() {
    return Iterator(&rules_, maxCharId, 0);
}

Grammar::ConstIterator Grammar::begin() const {
    ConstIterator res(&rules_, 0, 0);
    if (!res.isValid()) {
        ++res;
    }
    return res;
}

Grammar::ConstIterator Grammar::end() const {
    return ConstIterator(&rules_, maxCharId, 0);
}

Grammar::Iterator Grammar::begin(char c) {
    int id = c - 'A';
    return Iterator(&rules_, id, 0);
}

Grammar::Iterator Grammar::end(char c) {
    Iterator res(&rules_, c - 'A', rules_[c - 'A'].size());
    ++res;
    return res;
}

Grammar::ConstIterator Grammar::begin(char c) const {
    int id = c - 'A';
    auto cur = &rules_;
    return ConstIterator(cur, id, 0);
}

Grammar::ConstIterator Grammar::end(char c) const {
    ConstIterator res(&rules_, c - 'A', rules_[c - 'A'].size());
    ++res;
    return res;
}

Grammar::Iterator Grammar::eraseRule(Iterator it) {
    if (it.isValid()) {
        Iterator nxt = it;
        ++nxt;
        auto del_it = rules_[it.charId_].begin() + it.ruleId_;
        rules_[it.charId_].erase(del_it);
        --size_;
        return nxt;
    }
    return it;
}

#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <iostream>
#include <vector>
#include <string>
#include <set>

std::vector<std::string> parseRules(const std::string &rule);
bool isValidSingleRule(const std::string &rule);
bool isValidRule(const std::string &rule);
bool isNonTerminal(char c);
bool isSymbol(char c);

class Grammar {
    static const int maxCharId = 27;
    int size_;
    char start_;
    std::vector<std::vector<std::string>> rules_;

    template <bool isConst>
    class Iterator_: public std::bidirectional_iterator_tag {
    public:
        using pointer = typename std::conditional<isConst, const std::string*, std::string*>::type;
        using reference = typename std::conditional<isConst, const std::string&, std::string&>::type;
        using ptrType = typename std::conditional<isConst, const std::vector<std::vector<std::string>>*,
                std::vector<std::vector<std::string>>*>::type;

    private:
        int charId_, ruleId_;
        ptrType ptr_;
        bool isValid() const;

    public:
        explicit Iterator_(ptrType ptr = nullptr, int charId = 0, int ruleId = 0);
        Iterator_(const Iterator_ &other);
        Iterator_ &operator=(const Iterator_ &other);

        Iterator_ &operator++();
        Iterator_ &operator--();
        reference operator*();
        pointer operator->();
        bool operator==(const Iterator_& other) const;
        bool operator!=(const Iterator_& other) const;

        friend class Grammar;
    };

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

    using Iterator = Iterator_<false>;
    using ConstIterator = Iterator_<true>;

    Iterator eraseRule(Iterator it);

    Iterator begin();
    Iterator end();
    ConstIterator begin() const;
    ConstIterator end() const;

    Iterator begin(char c);
    Iterator end(char c);
    ConstIterator begin(char c) const;
    ConstIterator end(char c) const;
};

std::ostream& operator <<(std::ostream &stream, const Grammar &g);
std::istream& operator >>(std::istream& stream, Grammar &g);

#endif

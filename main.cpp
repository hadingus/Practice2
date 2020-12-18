#include <iostream>
#include "grammar_src/grammar.h"
#include "Earley_algo_src/algo.h"

int main() {
    std::cout << RuleVerifier::isValidRule("T->aTaB|1|aBa|");
    return 0;
}
#include <iostream>
#include "grammar_src/grammar.h"
#include "Earley_algo_src/algo.h"

int main() {
    Grammar g;
    std::cin >> g;
    g.deleteSimilarRules();
    std::cout << g;
    return 0;
}
#include <iostream>
#include <string>
#include "grammar_src/grammar.h"
#include "Earley_algo_src/algo.h"

int main() {

    Grammar g;
    std::cin >> g;
    g.deleteSimilarRules();
    EarleyAlgo solver(g);
    int n;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::string s;
        std::cin >> s;
        if (s == "1") {
            s = "";
        }
        std::cout << (solver.hasWord(s) ? "yes\n" : "no\n");
    }
    return 0;
}
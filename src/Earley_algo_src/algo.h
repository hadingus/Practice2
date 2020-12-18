#ifndef EARLEY_ALGO_H
#define EARLEY_ALGO_H

#include <iostream>
#include "grammar_src/grammar.h"

class EarleyAlgo {


    Grammar grammar_;
public:
    EarleyAlgo();
    EarleyAlgo(Grammar grammar);

};

#endif

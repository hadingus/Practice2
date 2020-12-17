project(algoSrc)

add_library(srcLib SHARED grammar_src/grammar.cpp Earley_algo_src/algo.cpp)

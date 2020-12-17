project(algoSrc)

include_directories(grammar_src/ Earley_algo_src/)
add_library(srcLib SHARED grammar_src/grammar.cpp Earley_algo_src/algo.cpp)

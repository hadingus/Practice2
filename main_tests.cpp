#include <iostream>
#include <gtest/gtest.h>
#include "grammar_src/grammar.h"

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
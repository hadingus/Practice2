#include <gtest/gtest.h>
#include "grammar_src/grammar.h"

TEST(GrammarTest, CharTest) {
    EXPECT_EQ(Grammar::isSymbol('a'), true);
    EXPECT_EQ(Grammar::isSymbol('3'), false);
}

TEST(GrammarTest, NonTerminalTest) {
    EXPECT_EQ(Grammar::isNonTerminal('A'), true);
    EXPECT_EQ(Grammar::isNonTerminal('2'), false);
    EXPECT_EQ(Grammar::isNonTerminal('v'), false);
    EXPECT_EQ(Grammar::isNonTerminal('F'), true);
}

TEST(GrammarTest, SingleRuleTest) {
    EXPECT_EQ(Grammar::isValidSingleRule("agfeg"), false);
    EXPECT_EQ(Grammar::isValidSingleRule("Alffmv"), false);
    EXPECT_EQ(Grammar::isValidSingleRule("a->frefgf"), false);
    EXPECT_EQ(Grammar::isValidSingleRule("A->"), false);
    EXPECT_EQ(Grammar::isValidSingleRule("A->flkrk"), true);
    EXPECT_EQ(Grammar::isValidSingleRule("A->1"), true);
    EXPECT_EQ(Grammar::isValidSingleRule("A->1flgkmek"), false);
    EXPECT_EQ(Grammar::isValidSingleRule("B-rgfr"), false);
    EXPECT_EQ(Grammar::isValidSingleRule("A->32f3krg"), false);
    EXPECT_EQ(Grammar::isValidSingleRule("A->egfr|kfre"), false);
    EXPECT_EQ(Grammar::isValidSingleRule("A->rkgrAflkgnvrfAGkr"), true);
}

TEST(GrammarTest, RuleTest) {
    EXPECT_EQ(Grammar::isValidRule("A->||"), false);
    EXPECT_EQ(Grammar::isValidRule("A->1|efglr|1"), true);
    EXPECT_EQ(Grammar::isValidRule("a->fek"), false);
    EXPECT_EQ(Grammar::isValidRule("A->fgF|fl|1"), true);
    EXPECT_EQ(Grammar::isValidRule("A->423|e"), false);
    EXPECT_EQ(Grammar::isValidRule("A->gkrdasFAF|fr2"), false);
    EXPECT_EQ(Grammar::isValidRule("G->1|feFdfsgAF|fr"), true);
}
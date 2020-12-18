#include <gtest/gtest.h>
#include "grammar_src/grammar.h"

TEST(RuleVerifierTest, parserTest) {
    auto q = RuleVerifier::parseRules("T->1|fkf|fEf");
    EXPECT_EQ(q.size(), 3);
    EXPECT_EQ(q[0], "T->1");
    EXPECT_EQ(q[1], "T->fkf");
    EXPECT_EQ(q[2], "T->fEf");
}

TEST(RuleVerifierTest, CharTest) {
    EXPECT_EQ(RuleVerifier::isSymbol('a'), true);
    EXPECT_EQ(RuleVerifier::isSymbol('3'), false);
}

TEST(RuleVerifierTest, NonTerminalTest) {
    EXPECT_EQ(RuleVerifier::isNonTerminal('A'), true);
    EXPECT_EQ(RuleVerifier::isNonTerminal('2'), false);
    EXPECT_EQ(RuleVerifier::isNonTerminal('v'), false);
    EXPECT_EQ(RuleVerifier::isNonTerminal('F'), true);
}

TEST(RuleVerifierTest, SingleRuleTest) {
    EXPECT_EQ(RuleVerifier::isValidSingleRule("agfeg"), false);
    EXPECT_EQ(RuleVerifier::isValidSingleRule("Alffmv"), false);
    EXPECT_EQ(RuleVerifier::isValidSingleRule("a->frefgf"), false);
    EXPECT_EQ(RuleVerifier::isValidSingleRule("A->"), false);
    EXPECT_EQ(RuleVerifier::isValidSingleRule("A->flkrk"), true);
    EXPECT_EQ(RuleVerifier::isValidSingleRule("A->1"), true);
    EXPECT_EQ(RuleVerifier::isValidSingleRule("A->1flgkmek"), false);
    EXPECT_EQ(RuleVerifier::isValidSingleRule("B-rgfr"), false);
    EXPECT_EQ(RuleVerifier::isValidSingleRule("A->32f3krg"), false);
    EXPECT_EQ(RuleVerifier::isValidSingleRule("A->egfr|kfre"), false);
    EXPECT_EQ(RuleVerifier::isValidSingleRule("A->rkgrAflkgnvrfAGkr"), true);
}

TEST(RuleVerifierTest, RuleTest) {
    EXPECT_EQ(RuleVerifier::isValidRule("A->||"), false);
    EXPECT_EQ(RuleVerifier::isValidRule("A->1|efglr|1"), true);
    EXPECT_EQ(RuleVerifier::isValidRule("a->fek"), false);
    EXPECT_EQ(RuleVerifier::isValidRule("A->fgF|fl|1"), true);
    EXPECT_EQ(RuleVerifier::isValidRule("A->423|e"), false);
    EXPECT_EQ(RuleVerifier::isValidRule("A->gkrdasFAF|fr2"), false);
    EXPECT_EQ(RuleVerifier::isValidRule("G->1|feFdfsgAF|fr"), true);
}

TEST(GrammarTest, addRuleTest) {
    Grammar g;
    EXPECT_EQ(g.addRule("fefgm"), false);
    EXPECT_EQ(g.size(), 0);
    EXPECT_EQ(g.addRule("T->1"), true);
    EXPECT_EQ(g.size(), 1);
    EXPECT_EQ(g.addRule("T->1|fegF|frF"), true);
    EXPECT_EQ(g.size(), 4);
}

TEST(GrammarTest, startTest) {
    Grammar g('T');
    EXPECT_EQ(g.getStart(), 'T');
    Grammar k;
    EXPECT_EQ(k.getStart(), 'S');
}

TEST(GrammarTest, initTest) {
    Grammar g('A');
    g.addRule("T->1|fdfs|ff");
    Grammar l(g);
    EXPECT_EQ(l.getStart(), 'A');
    EXPECT_EQ(l.size(), 3);

    Grammar p;
    p = l;

    EXPECT_EQ(p.getStart(), 'A');
    EXPECT_EQ(p.size(), 3);
}

TEST(GrammarTest, deleterTest) {
    Grammar g;
    g.addRule("T->f|fked|fkdgmkNFF");
    g.deleteSimilarRules();
    EXPECT_EQ(g.size(), 3);
    g.addRule("T->f");
    EXPECT_EQ(g.size(), 4);
    g.deleteSimilarRules();
    EXPECT_EQ(g.size(), 3);
    for (int i = 1; i < 100; ++i) {
        g.addRule("T->fFf");
    }
    g.deleteSimilarRules();
    EXPECT_EQ(g.size(), 4);
}

TEST(GrammarTest, operatorTest) {
    Grammar g;
    g.addRule("T->f|fked|fkdgmkNFF");
    EXPECT_EQ(g[0], "T->f");
    EXPECT_EQ(g[1], "T->fked");
    EXPECT_EQ(g[2], "T->fkdgmkNFF");
}

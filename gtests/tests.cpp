#include <gtest/gtest.h>
#include "grammar_src/grammar.h"

TEST(RuleTest, parserTest) {
    auto q = parseRules("T->1|fkf|fEf");
    EXPECT_EQ(q.size(), 3);
    std::vector<std::string> res = {"T->1", "T->fkf", "T->fEf"};
    int id = 0;
    for (const auto& it : q) {
        EXPECT_EQ(it, res[id++]);
    }
}

TEST(RuleTest, CharTest) {
    ASSERT_TRUE(isSymbol('a'));
    ASSERT_FALSE(isSymbol('3'));
    ASSERT_FALSE(isSymbol('A'));
}

TEST(RuleTest, NonTerminalTest) {
    ASSERT_TRUE(isNonTerminal('A'));
    ASSERT_FALSE(isNonTerminal('2'));
    ASSERT_FALSE(isNonTerminal('v'));
    ASSERT_TRUE(isNonTerminal('F'));
}

TEST(RuleTest, SingleRuleTest) {
    ASSERT_FALSE(isValidSingleRule("agfeg"));
    ASSERT_FALSE(isValidSingleRule("Alffmv"));
    ASSERT_FALSE(isValidSingleRule("a->frefgf"));
    ASSERT_FALSE(isValidSingleRule("A->"));
    ASSERT_TRUE(isValidSingleRule("A->flkrk"));
    ASSERT_TRUE(isValidSingleRule("A->1"));
    ASSERT_FALSE(isValidSingleRule("A->1flgkmek"));
    ASSERT_FALSE(isValidSingleRule("B-rgfr"));
    ASSERT_FALSE(isValidSingleRule("A->32f3krg"));
    ASSERT_FALSE(isValidSingleRule("A->egfr|kfre"));
    ASSERT_TRUE(isValidSingleRule("A->rkgrAflkgnvrfAGkr"));
}

TEST(RuleTest, RuleTest) {
    ASSERT_FALSE(isValidRule("A->||"));
    ASSERT_TRUE(isValidRule("A->1|efglr|1"));
    ASSERT_FALSE(isValidRule("a->fek"));
    ASSERT_TRUE(isValidRule("A->fgF|fl|1"));
    ASSERT_FALSE(isValidRule("A->423|e"));
    ASSERT_FALSE(isValidRule("A->gkrdasFAF|fr2"));
    ASSERT_TRUE(isValidRule("G->1|feFdfsgAF|fr"));
}

TEST(GrammarTest, addRuleTest) {
    Grammar g;
    ASSERT_FALSE(g.addRule("fefgm"));
    EXPECT_EQ(g.size(), 0);
    ASSERT_TRUE(g.addRule("T->1"));
    EXPECT_EQ(g.size(), 1);
    ASSERT_TRUE(g.addRule("T->1|fegF|frF"));
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

TEST(GrammarTest, iteratorTest) {
    Grammar g;
    g.addRule("T->g|1|gfdF");
    g.addRule("A->1");
    g.addRule("Z->1");
    int id = 0;
    for (auto it = g.begin('T'); it != g.end('T'); ++it) {
        id++;
    }
    EXPECT_EQ(id, 3);
    id = 0;
    for (auto i : g) {
        id++;
    }
    EXPECT_EQ(id, 5);
    auto it = g.begin();
    EXPECT_EQ(*it, "A->1");
}
#include <iostream>
#include <random>
#include <gtest/gtest.h>
#include "grammar_src/grammar.cpp"
#include "Earley_algo_src/algo.cpp"
#include <stack>
#include <fstream>
#include <fcntl.h>


std::mt19937 rnd;

TEST(RuleTest, parserTest) {
    auto q = parseRules("T->1|fkf|fEf");
    EXPECT_EQ(q.size(), 3);
    std::vector<std::string> res = {"T->", "T->fkf", "T->fEf"};
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
    ASSERT_TRUE(isValidSingleRule("A->"));
    ASSERT_TRUE(isValidSingleRule("A->flkrk"));
    ASSERT_TRUE(isValidSingleRule("A->1"));
    ASSERT_FALSE(isValidSingleRule("A->1flgkmek"));
    ASSERT_FALSE(isValidSingleRule("B-rgfr"));
    ASSERT_FALSE(isValidSingleRule("A->32f3krg"));
    ASSERT_FALSE(isValidSingleRule("A->egfr|kfre"));
    ASSERT_TRUE(isValidSingleRule("A->rkgrAflkgnvrfAGkr"));
}

TEST(RuleTest, RuleTest) {
    ASSERT_TRUE(isValidRule("A->||"));
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

TEST(GrammarTest, sizeTest) {
    Grammar g;
    EXPECT_EQ(g.size('S'), 0);
    g.addRule("S->sfwS|SS|a");
    EXPECT_EQ(g.size('S'), 3);
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
    EXPECT_EQ(*it, "A->");
}

TEST(GrammarTest, operatorTest) {
    Grammar g;
    g.addRule("T->g|1|gfdF");
    g.addRule("A->1");
    g.addRule("Z->1");
    Grammar p;
    p = g;
    EXPECT_EQ(p.size('T'), 3);
    EXPECT_EQ(p.size('A'), 1);
    EXPECT_EQ(p.size('S'), 0);
    EXPECT_EQ(p.size('q'), 0);
}

TEST(GrammarTest, eraseTest) {
    Grammar g;
    g.addRule("T->g|1|gfdF");
    g.addRule("A->1");
    g.addRule("Z->1");
    g.eraseRule(g.begin('T'));
    EXPECT_EQ(g.size(), 4);
    g.eraseRule(g.begin('G'));
    EXPECT_EQ(g.size(), 3);
    g.addRule("G->A|gf|rs");
    EXPECT_EQ(g.eraseRule(g.end()), g.end());
}

TEST(GrammarTest, constIterTest) {
    Grammar g;
    g.addRule("S->aSbSS|1");
    const Grammar &s = g;
    int id = 0;
    for (auto i : s) {
        ++id;
    }
    EXPECT_EQ(id, 2);
    g.addRule("A->sfs|fds");
    id = 0;
    for (auto it = s.begin('A'); it != s.end('A'); ++it) {
        ++id;
    }
    EXPECT_EQ(s.begin('G'), s.end('G'));
    EXPECT_EQ(id, 2);
}

TEST(GrammarTest, inputOutputTest) {
    int fd = open("tmp", O_WRONLY | O_CREAT, 0640);
    char str[] = "2\nS->As|aS|1\nA->sSA|s\n";
    write(fd, str, sizeof(str));
    close(fd);
    std::ifstream istream("tmp");
    Grammar g;
    istream >> g;
    EXPECT_EQ(g.size(), 5);
    EXPECT_EQ(g.size('A'), 2);
    std::ofstream ostream("tmp");
    ostream << g;
    remove("tmp");
}

bool isPsP(const std::string &s) {
    std::stack<char> q;
    for (char c : s) {
        if (c == 'a') {
            q.push(c);
        } else {
            if (q.empty() || q.top() != 'a') {
                return false;
            }
            q.pop();
        }
    }
    return q.empty();
}

TEST(EarleyTest, initTest) {
    EarleyAlgo solver;
    for (int iter = 0; iter < 100; ++iter) {
        int len = 1 + rnd() % 100;
        std::string s;
        for (int i = 0; i < len; ++i) {
            s += 'a' + (rnd() % 2);
        }
        EXPECT_FALSE(solver.hasWord(s));
    }
}

TEST(EarleyTest, simpleTest) {
    Grammar g;
    g.addRule("S->aSbS|");
    EarleyAlgo solver(g);
    ASSERT_TRUE(solver.hasWord("ab"));
    ASSERT_TRUE(solver.hasWord("abaabb"));
    ASSERT_FALSE(solver.hasWord("a"));
    for (int iter = 0; iter < 100; ++iter) {
        int len = 1 + rnd() % 100;
        std::string s;
        for (int i = 0; i < len; ++i) {
            s += 'a' + (rnd() % 2);
        }
        EXPECT_EQ(solver.hasWord(s), isPsP(s));
    }
}

TEST(EarleyTest, notSimpleTest) {
    Grammar g;
    g.addRule("S->SS|aSb|");
    EarleyAlgo solver(g);
    ASSERT_TRUE(solver.hasWord("ab"));
    ASSERT_TRUE(solver.hasWord("abaabb"));
    ASSERT_FALSE(solver.hasWord("a"));
    for (int iter = 0; iter < 100; ++iter) {
        int len = 1 + rnd() % 100;
        std::string s;
        for (int i = 0; i < len; ++i) {
            s += 'a' + (rnd() % 2);
        }
        EXPECT_EQ(solver.hasWord(s), isPsP(s));
    }
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
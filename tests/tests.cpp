#include <gtest/gtest.h>
#include "src/Grammar.h"
#include "src/ParserEarley.h"

TEST(Parser_test, Test1) {
  std::vector<Rule> rules;
  rules.emplace_back("S", "aBac");
  rules.emplace_back("B", "dB");
  rules.emplace_back("B", "");
  Grammar grammar(rules);
  ParserEarley parser(grammar);
  EXPECT_EQ(true, parser.Parse("aac"));
  EXPECT_EQ(true, parser.Parse("adddddac"));
  EXPECT_EQ(false, parser.Parse("aaccaa"));
  EXPECT_EQ(false, parser.Parse("a"));
  EXPECT_EQ(false, parser.Parse("aaa"));
}

TEST(Parser_test, Test2) {
  std::vector<Rule> rules;
  rules.emplace_back("S", "B");
  rules.emplace_back("B", "dBd");
  rules.emplace_back("B", "V");
  rules.emplace_back("V", "");
  rules.emplace_back("V", "aVa");
  Grammar grammar(rules);
  ParserEarley parser(grammar);
  EXPECT_EQ(true, parser.Parse("aa"));
  EXPECT_EQ(true, parser.Parse("dddaaddd"));
  EXPECT_EQ(false, parser.Parse("aaa"));
  EXPECT_EQ(false, parser.Parse("dadada"));
  EXPECT_EQ(true, parser.Parse("daaaad"));
}

TEST(Parser_test, Test3) {
  std::vector<Rule> rules;
  rules.emplace_back("S", "aSa");
  rules.emplace_back("S", "Bc");
  rules.emplace_back("B", "cB");
  rules.emplace_back("B", "");
  Grammar grammar(rules);
  ParserEarley parser(grammar);
  EXPECT_EQ(false, parser.Parse("aa"));
  EXPECT_EQ(true, parser.Parse("acccccca"));
  EXPECT_EQ(true, parser.Parse("aaaaacaaaaa"));
  EXPECT_EQ(false, parser.Parse("aacdcaa"));
  EXPECT_EQ(false, parser.Parse("aaaa"));
}
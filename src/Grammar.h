#ifndef PARSERS_SRC_GRAMMAR_H_
#define PARSERS_SRC_GRAMMAR_H_
#pragma once
#include <iostream>
#include "Rule.h"
#include <vector>

class Grammar {
 public:
  Grammar();

  std::vector<Rule> GetRulesWithNeededLeftPart(const std::string &needed_left_part);

  explicit Grammar(std::vector<Rule> rules_in);

  std::vector<Rule> rules;
  std::string starting_non_terminal = "S";
  std::string epsilon = "e";
  std::string master_starting_non_terminal = "M";
};

#endif //PARSERS_SRC_GRAMMAR_H_

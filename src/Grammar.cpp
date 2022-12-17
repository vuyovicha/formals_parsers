#pragma once
#include "Grammar.h"

Grammar::Grammar() {
  size_t rules_amount;
  std::cout << "Input amount of rules: ";
  std::cin >> rules_amount;
  std::cout << "Input rules - one per line (format without arrows: A a; A BC)\n";
  for (size_t i = 0; i < rules_amount; i++) {
    std::string left_part;
    std::string right_part;
    std::cin >> left_part >> right_part;
    if (right_part != epsilon) {
      rules.emplace_back(left_part, right_part);
    } else {
      rules.emplace_back(left_part, "");
    }
  }
  rules.insert(rules.begin(), Rule(master_starting_non_terminal, starting_non_terminal));
}


std::vector<Rule> Grammar::GetRulesWithNeededLeftPart(const std::string &needed_left_part) {
  std::vector<Rule> needed_rules;
  for (auto &rule : rules) {
    if (rule.left == needed_left_part) {
      needed_rules.push_back(rule);
    }
  }
  return needed_rules;
}

Grammar::Grammar(std::vector<Rule> rules_in) {
  rules = std::move(rules_in);
  rules.insert(rules.begin(), Rule(master_starting_non_terminal, starting_non_terminal));
}
#pragma once
#include "Rule.h"

Rule::Rule(std::string left_part, std::string right_part) {
  left = std::move(left_part);
  right = std::move(right_part);
}

std::vector<std::string> Rule::GetNonTerminalsFromRight() {
  std::vector<std::string> non_terminals;
  for (char &symbol : right) {
    if (std::isupper(symbol)) {
      non_terminals.emplace_back(std::string(1, symbol));
    }
  }
  return non_terminals;
};

bool Rule::operator<(const Rule &other) const {
  if (left == other.left) {
    return right < other.right;
  }
  return left < other.left;
}
#pragma once
#include "Rule.h"

Rule::Rule(std::string left_part, std::string right_part) {
  left = std::move(left_part);
  right = std::move(right_part);
}

bool Rule::operator<(const Rule &other) const {
  if (left == other.left) {
    return right < other.right;
  }
  return left < other.left;
}

bool Rule::operator==(const Rule &other) const {
  return left == other.left && right == other.right;
}
#pragma once
#include "Situation.h"

Situation::Situation(const Rule &rule_in, size_t dot_position_in, size_t index_in) {
  rule = rule_in;
  dot_position = dot_position_in;
  index = index_in;
}

bool Situation::operator<(const Situation &other) const {
  if (rule == other.rule) {
    if (dot_position == other.dot_position) {
      return index < other.index;
    } else {
      return dot_position < other.dot_position;
    }
  } else {
    return rule < other.rule;
  }
}

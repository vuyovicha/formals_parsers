#ifndef PARSERS_SRC_SITUATION_H_
#define PARSERS_SRC_SITUATION_H_
#pragma once
#include "Rule.h"

class Situation {
 public:
  Situation(const Rule &rule_in, size_t dot_position_in, size_t index_in);

  bool operator<(const Situation &other) const;

  Rule rule;
  size_t dot_position;
  size_t index;
};

#endif //PARSERS_SRC_SITUATION_H_

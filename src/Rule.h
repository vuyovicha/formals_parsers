#ifndef PARSERS_SRC_RULE_H_
#define PARSERS_SRC_RULE_H_
#pragma once
#include <string>

class Rule {
 public:
  Rule() = default;

  Rule(std::string left_part, std::string right_part);

  bool operator<(const Rule &other) const;

  bool operator==(const Rule &other) const;

  std::string left;
  std::string right;
};

#endif //PARSERS_SRC_RULE_H_

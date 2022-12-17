#ifndef PARSERS_SRC_RULE_H_
#define PARSERS_SRC_RULE_H_
#pragma once
#include <string>
#include <vector>

class Rule {
 public:
  Rule(std::string left_part, std::string right_part);

  std::vector<std::string> GetNonTerminalsFromRight();

  bool operator<(const Rule &other) const;

  std::string left;
  std::string right;
};

#endif //PARSERS_SRC_RULE_H_

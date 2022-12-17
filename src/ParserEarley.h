#ifndef PARSERS_SRC_PARSEREARLEY_H_
#define PARSERS_SRC_PARSEREARLEY_H_
#pragma once
#include "Grammar.h"
#include "Situation.h"
#include <set>

class ParserEarley {
 public:
  explicit ParserEarley(Grammar grammar) : grammar_(std::move(grammar)) {}

  void AnswerQueries();

  bool Parse(const std::string &word);

 private:
  void Scan(size_t situation_set_index, char symbol);

  void Predict(size_t situation_set_index);

  void Complete(size_t situation_set_index);

  std::vector<std::set<Situation>> situations_;
  Grammar grammar_;
};

#endif //PARSERS_SRC_PARSEREARLEY_H_

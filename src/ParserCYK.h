#ifndef PARSERS_SRC_PARSERCYK_H_
#define PARSERS_SRC_PARSERCYK_H_
#pragma once
#include "Grammar.h"

class ParserCYK {
 public:
  explicit ParserCYK(Grammar grammar) : grammar_(std::move(grammar)) {}

  void AnswerQueries();

  //do not consider master starting rule here (check for epsilon and delete M from non_terminals)
  bool Parse(const std::string &word);

 private:
  Grammar grammar_;
};

#endif //PARSERS_SRC_PARSERCYK_H_

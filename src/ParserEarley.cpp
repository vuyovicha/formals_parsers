#pragma once
#include "ParserEarley.h"

void ParserEarley::AnswerQueries() {
  size_t queries_amount;
  std::cout << "Input amount of words to check: ";
  std::cin >> queries_amount;
  for (size_t i = 0; i < queries_amount; i++) {
    std::cout << "Input word to check: ";
    std::string word;
    std::cin >> word;
    if (Parse(word)) {
      std::cout << "Word CAN be parsed by grammar!\n";
    } else {
      std::cout << "Word CANNOT be parsed by grammar!\n";
    }
  }
}

bool ParserEarley::Parse(const std::string &word) {
  situations_.clear();
  situations_.resize(1 + word.length());
  situations_[0].insert(Situation(grammar_.rules[0], 0, 0));
  for (size_t i = 0; i <= word.length(); i++) {
    if (i != 0) {
      Scan(i - 1, word[i - 1]);
    }
    bool set_changed = true;
    while (set_changed) {
      set_changed = false;
      size_t set_size = situations_[i].size();
      Predict(i);
      Complete(i);
      set_changed = (set_size != situations_[i].size());
    }
  }
  return situations_[word.length()].find(Situation(grammar_.rules[0], 1, 0)) != situations_[word.length()].end();
}

void ParserEarley::Scan(size_t situation_set_index, char symbol) {
  for (auto &situation : situations_[situation_set_index]) {
    if (symbol == situation.rule.right[situation.dot_position]) {
      situations_[situation_set_index + 1].insert(Situation(situation.rule, 1 + situation.dot_position, situation.index));
    }
  }
}

void ParserEarley::Predict(size_t situation_set_index) {
  for (auto &situation : situations_[situation_set_index]) {
    std::vector<Rule>
        needed_left_part_rules = grammar_.GetRulesWithNeededLeftPart(std::string(1, situation.rule.right[situation.dot_position]));
    for (auto &rule : needed_left_part_rules) {
      situations_[situation_set_index].insert(Situation(rule, 0, situation_set_index));
    }
  }
}

void ParserEarley::Complete(size_t situation_set_index) {
  for (auto &situation : situations_[situation_set_index]) {
    if (situation.dot_position == situation.rule.right.length()) {
      for (auto& new_situation : situations_[situation.index]) {
        if (situation.rule.left == std::string(1, new_situation.rule.right[new_situation.dot_position])) {
          situations_[situation_set_index].insert(Situation(new_situation.rule, 1 + new_situation.dot_position, new_situation.index));
        }
      }
    }
  }
}

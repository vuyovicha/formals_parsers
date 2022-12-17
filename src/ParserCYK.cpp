#pragma once
#include "ParserCYK.h"

void ParserCYK::AnswerQueries() {
  size_t queries_amount;
  std::cout << "Input amount of words to check: ";
  std::cin >> queries_amount;
  grammar_.ToCNF();
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

//do not consider master starting rule here (check for epsilon and delete M from non_terminals)
bool ParserCYK::Parse(const std::string &word) {
  if (word.length() == 0) {
    std::vector<Rule> terminal_rules = grammar_.GetTerminalRulesWithNeededLeft(grammar_.GetStartingNonTerminal());
    for (auto &rule : terminal_rules) {
      if (rule.right == grammar_.GetEpsilon()) {
        return true;
      }
    }
    return false;
  }
  std::set<std::string> non_terminals_set = grammar_.GetAllNonTerminals();
  std::vector<std::string> non_terminals;
  std::copy(non_terminals_set.begin(), non_terminals_set.end(), std::back_inserter(non_terminals));
  std::vector<std::vector<std::vector<bool>>> table
      (non_terminals.size(), std::vector<std::vector<bool>>(word.length(), std::vector<bool>(word.length(), false)));
  // for table elements like [A][i][i]
  for (size_t non_terminal_index = 0; non_terminal_index < non_terminals.size(); non_terminal_index++) {
    std::vector<Rule> terminal_rules = grammar_.GetTerminalRulesWithNeededLeft(non_terminals[non_terminal_index]);
    for (size_t word_index = 0; word_index < word.length(); word_index++) {
      for (auto &terminal_rule : terminal_rules) {
        if (std::string(1, word[word_index]) == terminal_rule.right) {
          table[non_terminal_index][word_index][word_index] = true;
          break;
        }
      }
    }
  }
  for (size_t index_difference = 1; index_difference < word.length(); index_difference++) {
    for (size_t non_terminal_index = 0; non_terminal_index < non_terminals.size(); non_terminal_index++) {
      std::vector<Rule> double_non_terminal_rules =
          grammar_.GetDoubleNonTerminalRulesWithNeededLeft(non_terminals[non_terminal_index]);
      for (size_t left_index = 0; left_index < word.length() - index_difference; left_index++) {
        size_t right_index = left_index + index_difference;
        for (auto &double_non_terminal_rule : double_non_terminal_rules) {
          size_t left_non_terminal_index =
              std::find(non_terminals.begin(), non_terminals.end(), std::string(1, double_non_terminal_rule.right[0]))
                  - non_terminals.begin();
          size_t right_non_terminal_index =
              std::find(non_terminals.begin(), non_terminals.end(), std::string(1, double_non_terminal_rule.right[1]))
                  - non_terminals.begin();
          for (size_t middle_index = left_index; middle_index < right_index; middle_index++) {
            table[non_terminal_index][left_index][right_index] = table[non_terminal_index][left_index][right_index] ||
                (table[left_non_terminal_index][left_index][middle_index]
                    && table[right_non_terminal_index][middle_index + 1][right_index]);

          }
        }
      }
    }
  }
  const std::string input_starting_non_terminal = "S";
  size_t input_starting_non_terminal_index =
      std::find(non_terminals.begin(), non_terminals.end(), input_starting_non_terminal)
          - non_terminals.begin();
  return table[input_starting_non_terminal_index][0][word.length() - 1];
}


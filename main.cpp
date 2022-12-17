#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <cctype>
#include <set>
#include <queue>

class Rule {
 public:
  Rule() = default;

  Rule(std::string left_part, std::string right_part) {
    left = std::move(left_part);
    right = std::move(right_part);
  }

  std::vector<std::string> GetNonTerminalsFromRight() {
    std::vector<std::string> non_terminals;
    for (char &symbol : right) {
      if (std::isupper(symbol)) {
        non_terminals.emplace_back(std::string(1, symbol));
      }
    }
    return non_terminals;
  };

  bool operator<(const Rule &other) const {
    if (left == other.left) {
      return right < other.right;
    }
    return left < other.left;
  }

  bool operator==(const Rule &other) const {
    return left == other.left && right == other.right;
  }

  std::string left;
  std::string right;
};

class Grammar {
 public:
  Grammar() {
    size_t rules_amount;
    std::cout << "Input amount of rules: ";
    std::cin >> rules_amount;
    std::cout << "Input rules - one per line (format without arrows: A a; A BC)\n";
    for (size_t i = 0; i < rules_amount; i++) {
      std::string left_part;
      std::string right_part;
      std::cin >> left_part >> right_part;
      rules.emplace_back(left_part, right_part);
    }
    rules.insert(rules.begin(), Rule(master_starting_non_terminal, starting_non_terminal));
  }

  std::vector<Rule> GetRulesWithNeededLeftPart(const std::string &needed_left_part) {
    std::vector<Rule> needed_rules;
    for (auto &rule : rules) {
      if (rule.left == needed_left_part) {
        needed_rules.push_back(rule);
      }
    }
    return needed_rules;
  }

  explicit Grammar(std::vector<Rule> rules_in) {
    rules = std::move(rules_in);
    rules.insert(rules.begin(), Rule(master_starting_non_terminal, starting_non_terminal));
  }

  std::string GetStartingNonTerminal() const {
    return starting_non_terminal;
  }

  std::string GetEpsilon() const {
    return epsilon;
  }

  std::vector<Rule> rules;
  std::string starting_non_terminal = "S";
  std::string epsilon = "e";
  std::string master_starting_non_terminal = "M";
};

// terminal and non_terminal symbols should not be the same even if they are in different case
// epsilon - e (so E is banned)
// Master start non_terminal - M (so m is banned)

class Situation {
 public:
  Situation(const Rule &rule_in, size_t dot_position_in, size_t index_in) {
    rule = rule_in;
    dot_position = dot_position_in;
    index = index_in;
  }

  bool operator<(const Situation &other) const {
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

  Rule rule;
  size_t dot_position;
  size_t index;
};

class ParserEarley {
 public:
  explicit ParserEarley(Grammar grammar) : grammar_(std::move(grammar)) {}

  void AnswerQueries() {
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

 private:
  //do not consider master starting rule here (check for epsilon and delete M from non_terminals)
  bool Parse(const std::string &word) {
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
    return situations_[word.size()].find(Situation(grammar_.rules[0], 1, 0)) != situations_[word.size()].end();
  }

  void Scan(size_t situation_set_index, char symbol) {
    for (auto &situation : situations_[situation_set_index]) {
      if (symbol == situation.rule.right[situation.dot_position]) {
        situations_[situation_set_index].insert(Situation(situation.rule, 1 + situation.dot_position, situation.index));
      }
    }
  }

  void Predict(size_t situation_set_index) {
    for (auto &situation : situations_[situation_set_index]) {
      std::vector<Rule>
          needed_left_part_rules = grammar_.GetRulesWithNeededLeftPart(situation.rule.right[situation.dot_position]);
      for (auto &rule : needed_left_part_rules) {
        situations_[situation_set_index].insert(Situation(rule, 0, situation_set_index));
      }
    }
  }

  void Complete(size_t situation_set_index) {
    for (auto &situation : situations_[situation_set_index]) {
      if (situation.dot_position == situation.rule.right.length()) {
        for (auto& new_situation : situations_[situation.index]) {
          if (situation.rule.left == new_situation.rule.right[new_situation.dot_position]) {
            situations_[situation_set_index].insert(Situation(new_situation.rule, 1 + new_situation.dot_position, new_situation.index));
          }
        }
      }
    }
  }

  std::vector<std::set<Situation>> situations_;
  Grammar grammar_;
};

int main() {
  Grammar grammar;
  ParserEarley parser(grammar);
  parser.AnswerQueries();
  return 0;
}

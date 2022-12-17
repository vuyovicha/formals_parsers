#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <cctype>
#include <set>
#include <queue>

class Rule {
 public:
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
    all_non_terminals = GetAllNonTerminals();
  }

  explicit Grammar(std::vector<Rule> rules_in) {
    rules = std::move(rules_in);
    all_non_terminals = GetAllNonTerminals();
  }

  void ToCNF() {
    RemoveNonGenerating();
    RemoveUnreachable();
    RemoveMixedRules();
    RemoveLongRules();
    RemoveEpsilonGeneratingRules();
    RemoveRepeatingRules();
    RemoveSingleRules();
    RemoveRepeatingRules();
    // remove rules from master starting non_terminals
  }

  std::string GetStartingNonTerminal() const {
    return starting_non_terminal;
  }

  std::string GetEpsilon() const {
    return epsilon;
  }

  std::set<std::string> GetAllNonTerminals() {
    std::set<std::string> non_terminals;
    for (auto &rule : rules) {
      non_terminals.insert(rule.left);
      std::vector<std::string> rule_right_non_terminals = rule.GetNonTerminalsFromRight();
      for (auto &non_terminal : rule_right_non_terminals) {
        non_terminals.insert(non_terminal);
      }
    }
    return non_terminals;
  }

  // rules like A -> b
  std::vector<Rule> GetTerminalRulesWithNeededLeft(const std::string &needed_left_part) {
    std::vector<Rule> needed_rules;
    for (auto &rule : rules) {
      if (rule.left == needed_left_part) {
        if (rule.right.length() == 1 && std::islower(rule.right[0])) {
          needed_rules.push_back(rule);
        }
      }
    }
    return needed_rules;
  }

  // rules like A -> BC
  std::vector<Rule> GetDoubleNonTerminalRulesWithNeededLeft(const std::string &needed_left_part) {
    std::vector<Rule> needed_rules;
    for (auto &rule : rules) {
      if (rule.left == needed_left_part) {
        if (rule.right.length() == 2 && std::isupper(rule.right[0]) && std::isupper(rule.right[1])) {
          needed_rules.push_back(rule);
        }
      }
    }
    return needed_rules;
  }

  void PrintGrammar() {
    std::cout << "\nGRAMMAR:\n";
    for (auto& rule : rules) {
      std::cout << rule.left << " -> " << rule.right << std::endl;
    }
    std::cout << std::endl;
  }

 private:
  void RemoveRepeatingRules() {
    std::set<Rule> rules_set(rules.begin(), rules.end());
    std::vector<Rule> new_rules;
    std::copy(rules_set.begin(), rules_set.end(), std::back_inserter(new_rules));
    rules = new_rules;
  }

  void RemoveSingleRules() {
    std::set<std::pair<std::string, std::string>> chain_pairs = GetAllChainPairs();
    // do not remove starting symbol chain rule here (e. g. M -> S)
    for (auto &chain_pair : chain_pairs) {
      if (chain_pair.first != master_starting_non_terminal) {
        std::vector<Rule> non_chain_rules_with_needed_left_part = GetAllNonSingleRulesWithNeededLeft(chain_pair.second);
        for (auto &non_chain_rule : non_chain_rules_with_needed_left_part) {
          rules.emplace_back(chain_pair.first, non_chain_rule.right);
        }
      }
    }
    DeleteSingleRules();
  }

  void DeleteSingleRules() {
    auto rules_iterator = rules.begin();
    while (rules_iterator != rules.end()) {
      if ((*rules_iterator).left != master_starting_non_terminal && (*rules_iterator).right.length() == 1 && std::isupper((*rules_iterator).right[0])) {
        rules_iterator = rules.erase(rules_iterator);
      } else {
        rules_iterator++;
      }
    }
  }

  std::vector<Rule> GetAllNonSingleRulesWithNeededLeft(const std::string &needed_left_part) {
    std::vector<Rule> needed_rules;
    for (auto &rule : rules) {
      if (rule.left == needed_left_part) {
        if (!(rule.right.length() == 1 && std::isupper(rule.right[0]))) {
          needed_rules.push_back(rule);
        }
      }
    }
    return needed_rules;
  }

  std::set<std::pair<std::string, std::string>> GetAllChainPairs() {
    std::set<std::pair<std::string, std::string>> chain_pairs = GetBaseChainPairs();
    std::queue<std::pair<std::string, std::string>> chain_pairs_queue;
    std::set<Rule> single_rules = GetALlSingleRules(); // A -> B
    for (const auto &chain : chain_pairs) {
      chain_pairs_queue.push(chain);
    }
    while (!chain_pairs_queue.empty()) {
      std::pair<std::string, std::string> current_chain_pair = chain_pairs_queue.front();
      for (auto &single_rule : single_rules) {
        if (single_rule.left == current_chain_pair.second) {
          size_t set_size = chain_pairs.size();
          std::pair<std::string, std::string> new_pair = std::make_pair(current_chain_pair.first, single_rule.right);
          chain_pairs.insert(new_pair);
          if (set_size != chain_pairs.size()) {
            chain_pairs_queue.push(new_pair);
          }
        }
      }
      chain_pairs_queue.pop();
    }
    return chain_pairs;
  }

  std::set<Rule> GetALlSingleRules() {
    std::set<Rule> single_rules;
    for (auto &rule : rules) {
      if (rule.right.length() == 1 && std::isupper(rule.right[0])) {
        single_rules.insert(rule);
      }
    }
    return single_rules;
  }

  std::set<std::pair<std::string, std::string>> GetBaseChainPairs() {
    std::set<std::string> non_terminals = GetAllNonTerminals();
    std::set<std::pair<std::string, std::string>> chain_pairs;
    // added base chain pairs here, e. g. <A, A>
    for (auto &non_terminal : non_terminals) {
      chain_pairs.insert(std::make_pair(non_terminal, non_terminal));
    }
    return chain_pairs;
  }

  void RemoveNonGenerating() {
    std::set<std::string> generating;
    for (auto &rule : rules) {
      if (rule.GetNonTerminalsFromRight().empty()) {
        generating.insert(rule.left);
      }
    }
    bool generating_changed = true;
    while (generating_changed) {
      generating_changed = false;
      for (auto &rule : rules) {
        std::vector<std::string> non_terminals = rule.GetNonTerminalsFromRight();
        bool non_terminals_in_set = true;
        for (auto &non_terminal : non_terminals) {
          if (generating.find(non_terminal) == generating.end()) {
            non_terminals_in_set = false;
          }
        }
        if (non_terminals_in_set) {
          size_t size = generating.size();
          generating.insert(rule.left);
          if (!generating_changed) {
            generating_changed = (size != generating.size());
          }
        }
      }
    }
    RemoveSetClosureNonTerminals(generating);
  };

  void RemoveSetClosureNonTerminals(const std::set<std::string> &set_non_terminals) {
    std::set<std::string> closure_set;
    for (auto &non_terminal : all_non_terminals) {
      if (set_non_terminals.find(non_terminal) == set_non_terminals.end()) {
        closure_set.insert(non_terminal);
      }
    }
    auto rules_iterator = rules.begin();
    while (rules_iterator != rules.end()) {
      if (closure_set.find((*rules_iterator).left) != closure_set.end()) {
        rules_iterator = rules.erase(rules_iterator);
        continue;
      } else {
        std::vector<std::string> non_terminals = (*rules_iterator).GetNonTerminalsFromRight();
        for (auto &non_terminal : non_terminals) {
          if (closure_set.find(non_terminal) != closure_set.end()) {
            rules_iterator = rules.erase(rules_iterator);
            continue;
          }
        }
      }
      rules_iterator++;
    }
  }

  void RemoveUnreachable() {
    std::set<std::string> reachable;
    reachable.insert(starting_non_terminal);
    bool reachable_changed = true;
    while (reachable_changed) {
      reachable_changed = false;
      for (auto &rule : rules) {
        if (reachable.find(rule.left) != reachable.end()) {
          std::vector<std::string> non_terminals = rule.GetNonTerminalsFromRight();
          size_t reachable_size = reachable.size();
          for (auto &non_terminal : non_terminals) {
            reachable.insert(non_terminal);
          }
          if (!reachable_changed) {
            reachable_changed = (reachable.size() != reachable_size);
          }
        }
      }
    }
    RemoveSetClosureNonTerminals(reachable);
  }

  void RemoveMixedRules() {
    bool rules_changed = true;
    while (rules_changed) {
      rules_changed = false;
      for (auto &rule : rules) {
        if (rule.right.length() > 1) {
          for (char &symbol : rule.right) {
            if (std::islower(symbol)) {
              rules_changed = true;
              rules.emplace_back(std::string(1, std::toupper(symbol)), std::string(1, symbol));
              ReplaceTerminals(symbol, std::toupper(symbol));
            }
          }
        }
      }
    }
  }

  void ReplaceTerminals(char from_symbol, char to_symbol) {
    for (auto &rule : rules) {
      if (rule.right.length() > 1) {
        for (char &symbol : rule.right) {
          if (symbol == from_symbol) {
            symbol = to_symbol;
          }
        }
      }
    }
  }

  void RemoveLongRules() {
    std::vector<std::string> unused_non_terminals = GetUnusedNonTerminals();
    bool rules_changed = true;
    while (rules_changed) {
      rules_changed = false;
      for (size_t i = 0; i < rules.size(); i++) {
        if (rules[i].right.length() > 2) {
          std::string new_non_terminal = unused_non_terminals.back();
          unused_non_terminals.pop_back();
          rules.emplace_back(rules[i].left, rules[i].right[0] + new_non_terminal);
          rules.emplace_back(new_non_terminal, rules[i].right.substr(1));
          rules.erase(rules.begin() + i);
          rules_changed = true;
          break;
        }
      }
    }
  }

  std::set<std::string> GetEpsilonGeneratingNonTerminals() {
    std::set<std::string> epsilon_generating;
    for (auto &rule : rules) {
      if (rule.right == epsilon) {
        epsilon_generating.insert(rule.left);
      }
    }
    bool set_changed = true;
    while (set_changed) {
      set_changed = false;
      for (auto &rule : rules) {
        std::vector<std::string> non_terminals = rule.GetNonTerminalsFromRight();
        if (rule.right.length() == non_terminals.size()) {
          bool all_non_terminals_present_in_set = true;
          for (auto &non_terminal : non_terminals) {
            if (epsilon_generating.find(non_terminal) == epsilon_generating.end()) {
              all_non_terminals_present_in_set = false;
              break;
            }
          }
          if (all_non_terminals_present_in_set) {
            size_t set_size = epsilon_generating.size();
            epsilon_generating.insert(rule.left);
            if (!set_changed) {
              set_changed = (set_size != epsilon_generating.size());
            }
          }
        }
      }
    }
    return epsilon_generating;
  }

  void RemoveStraightEpsilonRules() {
    bool rules_changed = true;
    while (rules_changed) {
      rules_changed = false;
      for (size_t i = 0; i < rules.size(); i++) {
        if (rules[i].right == epsilon) {
          rules.erase(rules.begin() + i);
          rules_changed = true;
          break;
        }
      }
    }
  }

  void RemoveEpsilonGeneratingRules() {
    std::set<std::string> epsilon_generating_non_terminals = GetEpsilonGeneratingNonTerminals();
    size_t rules_amount = rules.size();
    for (size_t i = 0; i < rules_amount; i++) {
      if (rules[i].right.length() == 2) {
        std::vector<std::string> non_terminals = rules[i].GetNonTerminalsFromRight();
        if (epsilon_generating_non_terminals.find(non_terminals[0]) != epsilon_generating_non_terminals.end()) {
          rules.emplace_back(rules[i].left, rules[i].right.substr(1));
        }
        if (epsilon_generating_non_terminals.find(non_terminals[1]) != epsilon_generating_non_terminals.end()) {
          rules.emplace_back(rules[i].left, rules[i].right.substr(0, 1));
        }
      }
    }
    RemoveStraightEpsilonRules();
    if (epsilon_generating_non_terminals.find(starting_non_terminal) != epsilon_generating_non_terminals.end()) {
      rules.emplace_back(master_starting_non_terminal, starting_non_terminal);
      rules.emplace_back(master_starting_non_terminal, epsilon);
      starting_non_terminal = master_starting_non_terminal;
    }
  }

  std::vector<std::string> GetUnusedNonTerminals() {
    const size_t kAlphabetLength = 26;
    std::vector<bool> symbols_usage_flags(kAlphabetLength, false);
    //0 index - letter a, 25 index - letter z
    for (auto &rule : rules) {
      symbols_usage_flags[std::toupper(rule.left[0]) - 'A'] = true;
      for (char &symbol : rule.right) {
        symbols_usage_flags[std::toupper(symbol) - 'A'] = true;
      }
    }
    symbols_usage_flags['M' - 'A'] = true; // master starting non_terminal!
    std::vector<std::string> unused_non_terminals;
    for (size_t i = 0; i < symbols_usage_flags.size(); i++) {
      if (!symbols_usage_flags[i]) {
        unused_non_terminals.emplace_back(1, 'A' + i);
      }
    }
    return unused_non_terminals;
  }

  std::vector<Rule> rules;
  std::string starting_non_terminal = "S";
  std::set<std::string> all_non_terminals;
  std::string epsilon = "e";
  std::string master_starting_non_terminal = "M";
};

// terminal and non_terminal symbols should not be the same even if they are in different case
// epsilon - e (so E is banned)
// Master start non_terminal - M (so m is banned)

class ParserCYK {
 public:
  explicit ParserCYK(Grammar grammar) : grammar_(std::move(grammar)) {
    grammar_.ToCNF();
  }

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

  Grammar grammar_;
};

int main() {
  Grammar grammar;
  ParserCYK parser(grammar);
  parser.AnswerQueries();
  return 0;
}

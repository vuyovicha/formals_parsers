#ifndef PARSERS_SRC_GRAMMAR_H_
#define PARSERS_SRC_GRAMMAR_H_
#pragma once
#include "Rule.h"
#include <iostream>
#include <set>
#include <queue>

class Grammar {
 public:
  explicit Grammar();

  explicit Grammar(std::vector<Rule> rules_in);

  void ToCNF();

  std::string GetStartingNonTerminal() const;

  std::string GetEpsilon() const;

  std::set<std::string> GetAllNonTerminals();

  // rules like A -> b
  std::vector<Rule> GetTerminalRulesWithNeededLeft(const std::string &needed_left_part);

  // rules like A -> BC
  std::vector<Rule> GetDoubleNonTerminalRulesWithNeededLeft(const std::string &needed_left_part);

  void PrintGrammar();

 private:
  void RemoveRepeatingRules();

  void RemoveSingleRules();

  void DeleteSingleRules();

  std::vector<Rule> GetAllNonSingleRulesWithNeededLeft(const std::string &needed_left_part);

  std::set<std::pair<std::string, std::string>> GetAllChainPairs();

  std::set<Rule> GetALlSingleRules();

  std::set<std::pair<std::string, std::string>> GetBaseChainPairs();

  void RemoveNonGenerating();

  void RemoveSetClosureNonTerminals(const std::set<std::string> &set_non_terminals);

  void RemoveUnreachable();

  void RemoveMixedRules();

  void ReplaceTerminals(char from_symbol, char to_symbol);

  void RemoveLongRules();

  std::set<std::string> GetEpsilonGeneratingNonTerminals();

  void RemoveStraightEpsilonRules();

  void RemoveEpsilonGeneratingRules();

  std::vector<std::string> GetUnusedNonTerminals();

  std::vector<Rule> rules;
  std::string starting_non_terminal = "S";
  std::set<std::string> all_non_terminals;
  std::string epsilon = "e";
  std::string master_starting_non_terminal = "M";
};

#endif //PARSERS_SRC_GRAMMAR_H_

#include "src/Grammar.h"
#include "src/ParserEarley.h"

// terminal and non_terminal symbols should not be the same even if they are in different case
// epsilon - e (so E is banned)
// Master start non_terminal - M (so m is banned)

int main() {
  Grammar grammar;
  ParserEarley parser(grammar);
  parser.AnswerQueries();
  return 0;
}

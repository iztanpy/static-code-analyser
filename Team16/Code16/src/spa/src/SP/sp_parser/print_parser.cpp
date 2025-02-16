#include "SP/sp_parser/print_parser.h"

int PrintParser::parse(std::vector<Token>& tokens) {
  // Check if there are enough tokens for a valid print statement
  if (index + 3 > tokens.size()) {
    return -1;
  }

  Token printNameToken = tokens[index + 1];
  Token semicolonToken = tokens[index + 2];

  // Define the set of valid keyword token types
  std::unordered_set < TokenType > validKeywords = {
      TokenType::kEntityAssign, TokenType::kEntityProcedure,
      TokenType::kEntityRead, TokenType::kEntityPrint,
      TokenType::kEntityWhile, TokenType::kEntityIf,
      TokenType::kEntityElse, TokenType::kEntityCall,
      TokenType::kEntityStmt, TokenType::kEntityConstant,
      TokenType::kEntityVariable
  };

  // Check if the print name token is a keyword and convert it to a literal if necessary
  if (validKeywords.find(printNameToken.tokenType) != validKeywords.end()) {
    printNameToken.tokenType = TokenType::kLiteralName;
  }

  // Check if the print name is a literal
  if (printNameToken.tokenType != TokenType::kLiteralName) {
    return -1;
  }

  // Check if the print statement is terminated correctly
  if (semicolonToken.tokenType != TokenType::kSepSemicolon) {
    return -1;
  }

  // Update the value of the 'print' token to match the print name
  Token print = tokens[index];
  print.value = printNameToken.value;

  // Update the current index and create the AST node
  index = index + 3;
  std::shared_ptr<TNode> root = TNodeFactory::createNode(print, lineNumber);
  design_extractor::extractDesign(root, visitor);
  followsStatementStack.top().insert(lineNumber);
  cfg::handleStatement(lineNumber);

  lineNumber++;
  return index;
}

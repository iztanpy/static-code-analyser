#include "SP/sp_parser/read_parser.h"

int read_parser::parse(std::vector<Token>& tokens) {
  // Check if there are enough tokens for a valid read statement
  if (index + 3 > tokens.size()) {
    return -1;
  }

  // Validate read name
  Token readNameToken = tokens[index + 1];
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

  // Check if the read name token is a keyword and convert it to a literal if necessary
  if (validKeywords.find(readNameToken.tokenType) != validKeywords.end()) {
    readNameToken.tokenType = TokenType::kLiteralName;
  }

  // Check if the read name is a literal
  if (readNameToken.tokenType != TokenType::kLiteralName) {
    return -1;
  }

  // Check if the read statement is terminated correctly
  if (semicolonToken.tokenType != TokenType::kSepSemicolon) {
    return -1;
  }

  // Update the value of the 'read' token to match the read name
  Token read = tokens[index];
  read.value = readNameToken.value;

  // Update the current index and create the AST node
  index = index + 3;
  std::shared_ptr<TNode> root = TNodeFactory::createNode(read, lineNumber);
  design_extractor::extractDesign(root, visitor);
  followsStatementStack.top().insert(lineNumber);
  cfg::handleStatement(lineNumber);

  lineNumber++;

  return index;
}

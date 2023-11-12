#include "SP/sp_parser/procedure_parser.h"

#include <utility>

int ProcedureParser::parse(std::vector<Token>& tokens) {
  if (isParsingProcedure) throw InvalidSyntaxError();
  // validate procedure declaration: procedure (already validated), name, open brace
  if (Parser::index + 2 > tokens.size()) {
    return -1;
  }
  // validate procedure name
  Token procedureNameToken = tokens[Parser::index + 1];
  // check if name is keyword
  std::unordered_set < TokenType > keywords = {TokenType::kEntityAssign, TokenType::kEntityProcedure,
                                               TokenType::kEntityRead, TokenType::kEntityPrint,
                                               TokenType::kEntityWhile, TokenType::kEntityIf,
                                               TokenType::kEntityElse, TokenType::kEntityCall,
                                               TokenType::kEntityStmt, TokenType::kEntityConstant,
                                               TokenType::kEntityVariable};

  if (keywords.find(procedureNameToken.tokenType) != keywords.end()) {
    // set token to literal
    procedureNameToken.tokenType = TokenType::kLiteralName;
  }
  // check that name is literal
  if (procedureNameToken.tokenType != TokenType::kLiteralName) {
    return -1;
  }

  // validate procedure open brace
  size_t openBracesIndex = Parser::index + 2;
  if (tokens[openBracesIndex].tokenType != TokenType::kSepOpenBrace) {
    return -1;
  }

  // build procedure ast
  Token procedure = tokens[Parser::index];
  procedure.value = procedureNameToken.value;
  // set current procedure name
  parse_utils::setProcedureName(procedure.value);
  std::shared_ptr<TNode> root = TNodeFactory::createNode(procedure, lineNumber);
  cfg::addToCfgNodeMap(procedure.value);

  // set root node
  design_extractor::extractDesign(root, visitor);

  Parser::index = Parser::index + 3;

  std::set<int> procedureFollowsSet;
  followsStatementStack.push(procedureFollowsSet);
  isParsingProcedure = true;
  return Parser::index;
}

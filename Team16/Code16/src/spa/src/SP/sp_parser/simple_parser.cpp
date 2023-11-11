#include <iostream>
#include <vector>
#include "SP/sp_parser/simple_parser.h"

simple_parser::simple_parser(ASTVisitor* astVisitorPtr) : visitor(astVisitorPtr) {}

int simple_parser::parse(std::vector<Token>& tokens) {
  reset();
  while (index < tokens.size()) {
    Token* curr_token = &tokens[index];
    bool isWhileParent = !controlStructureStack.empty() && controlStructureStack.top() == "while";
    bool isIfParent = !controlStructureStack.empty() && (
        controlStructureStack.top() == "if" || controlStructureStack.top() == "else");
    int parentStatementNumber = !parentStatementStack.empty() ? parentStatementStack.top() : -1;
    if ((isWhileParent || isIfParent) && parentStatementNumber != -1
        && curr_token->tokenType != TokenType::kSepCloseBrace) {
      visitor->setParentStatementNumberMap(parentStatementNumber, lineNumber);
    }

    if (followsStatementStack.empty() && tokens[index].getValue() != "procedure") {
      throw InvalidSyntaxError();
    }
    parser = factory->createParser(curr_token->tokenType, lineNumber, index);
    parser->start_parse(tokens, index);
  }
  checkCalls();
  return index;
}

void simple_parser::checkCalls() {
  for (const auto& callerCallee : visitor->getCallerCalleeHashmap()) {
    const std::unordered_set<std::string>& calleeSet = callerCallee.second;
    for (const std::string& callee : calleeSet) {
      // Check if callee exists in procedureLabels
      std::string test = callee;
      std::set < std::string > procLabels = visitor->getProcedureLabels();
      int count = procLabels.count(callee);
      if (procLabels.count(callee) == 0) {
        throw InvalidSemanticError();
      }
    }
  }
}

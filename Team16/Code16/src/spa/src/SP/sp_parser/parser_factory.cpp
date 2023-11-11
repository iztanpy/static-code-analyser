#include "SP/sp_parser/parser_factory.h"

ParserFactory::ParserFactory(ASTVisitor* astVisitorPtr) : visitor(astVisitorPtr) {}

void ParserFactory::setUpParserMap() {
  parserMap[TokenType::kLiteralName] = assignmentParser;
  parserMap[TokenType::kEntityProcedure] = procedureParser;
  parserMap[TokenType::kEntityRead] = readParser;
  parserMap[TokenType::kEntityWhile] = whileParser;
  parserMap[TokenType::kEntityPrint] = printParser;
  parserMap[TokenType::kEntityIf] = ifParser;
  parserMap[TokenType::kEntityElse] = elseParser;
  parserMap[TokenType::kEntityCall] = callParser;
  parserMap[TokenType::kSepCloseBrace] = closeBraceParser;
}

Parser* ParserFactory::createParser(TokenType tokenType, int lineNumber, int curr_index) {
  if (parserMap.empty()) {
    setUpParserMap();
  }
  if (parserMap.find(tokenType) != parserMap.end()) {
    Parser* parser = parserMap[tokenType];
    parser->setLineNumber(lineNumber);
    parser->setIndex(curr_index);
    return parserMap[tokenType];
  } else {
    throw InvalidSyntaxError();
  }
}

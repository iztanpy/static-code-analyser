#include "SP/source_processor.h"

SourceProcessor::SourceProcessor(write_facade* writeFacadePtr) :
    writeFacade(writeFacadePtr), simpleParser(&this->visitor) {}

ASTVisitor SourceProcessor::getVisitor() const {
  return visitor;
}

void SourceProcessor::processSource(const std::string& simpleProgram) {
  std::vector<struct Token> tokens = tokeniser.tokenise(simpleProgram);
  try {
    simpleParser.parse(tokens);
    design_extractor::populatePKB(&this->visitor, writeFacade);
  } catch (const InvalidSyntaxError& e) {
    e.log();
  } catch (const InvalidTokenTypeError& e) {
    e.log();
  } catch (const InvalidSemanticError& e) {
    e.log();
  }
}

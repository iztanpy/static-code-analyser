#include "SP/sp_parser/close_brace_parser.h"

int close_brace_parser::parse(std::vector<Token>& tokens) {
  if (index - 1 > 0 && tokens[index - 1].tokenType == TokenType::kSepOpenBrace) {
    throw InvalidSyntaxError();
  }
  std::set<int> top_set = followsStatementStack.top();
  insertFollowsHashMap(top_set);
  followsStatementStack.pop();
  if (!controlStructureStack.empty() && controlStructureStack.top() == "while" && currWhileDepth >= 1) {
    cfg::handleEndWhileStatement();  // End of While Block CFG
    controlStructureStack.pop();  // Pop the 'while'
    parentStatementStack.pop();  // Pop the parent statement
    currWhileDepth--;  // Decrease the depth
  } else if (!controlStructureStack.empty() && controlStructureStack.top() == "else" && currIfDepth >= 1) {
    cfg::handleEndElseStatement();  // End of Else Block CFG
    currIfDepth--;  // Decrease the depth
    controlStructureStack.pop();  // Pop the 'else'
    controlStructureStack.pop();  // Pop the 'if'
    parentStatementStack.pop();  // Pop the parent statement
  } else if (!controlStructureStack.empty() && controlStructureStack.top() == "if" && currIfDepth >= 1) {
    bool hasElse = index + 2 < tokens.size()
        && tokens[index + 1].getValue() == "else"
        && tokens[index + 2].tokenType == TokenType::kSepOpenBrace;
    if (hasElse) {  // End of if Block CFG
      cfg::handleEndIfStatement(hasElse);
      index += 1;
      return index;
    } else {  // End of if Block CFG no else!
      cfg::handleEndIfStatement(hasElse);
      currIfDepth--;  // Decrease the depth
      controlStructureStack.pop();  // Pop the 'if'
      parentStatementStack.pop();  // Pop the parent
    }
  } else {  // other cases which have brackets
    if (!controlStructureStack.empty() && currWhileDepth > 0) {
      currWhileDepth--;  // Decrease the depth
      currIfDepth--;  // Decrease the depth
    } else {  // end of procedure
      visitor->setProcedureLineNumberMap(parse_utils::getProcedureName(), lineNumber - 1);
      cfg::handleEndProcedureStatement();
      isParsingProcedure = false;
    }
  }
  index += 1;
  return index;
}

void close_brace_parser::insertFollowsHashMap(std::set<int> followsSet) {
  if (followsSet.size() > 1) {
    auto it = followsSet.begin();
    auto nextIt = std::next(it);

    // Iterate up to the second-to-last element
    while (nextIt != followsSet.end()) {
      // Compare *it and *nextIt
      int before = *it;
      int after = *nextIt;
      visitor->setFollowStatementNumberMap(before, after);

      ++it;
      ++nextIt;
    }
  }
}

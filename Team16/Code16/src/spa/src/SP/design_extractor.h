#pragma once

// C system headers
#include <stdio.h>

// C++ system headers
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <string>

// Other library or external headers
#include "SP/visitor.h"
#include "SP/tnode.h"

class TNode;
class ASTVisitor;

/**
 * @class design_extractor
 * @brief A class responsible for extracting and populating design information from the abstract syntax tree (AST).
 *
 * The `design_extractor` class is responsible for extracting and populating design-related information
 * from the abstract syntax tree (AST) generated during the analysis of code written in the Simple Programming Language (SPL).
 * It works in conjunction with an `ASTVisitor` to traverse the AST and gather relevant data.
 */
class design_extractor {
 public:
  /**
   * @brief Constructs a design_extractor object.
  */
  design_extractor() = default;
  /**
    * @brief Extracts design information from the AST and populates the Program Knowledge Base (PKB).
    *
    * This method is responsible for extracting design-related information from the abstract syntax tree (AST)
    * and populating the Program Knowledge Base (PKB). It performs the necessary analysis and data collection
    * to enable subsequent program analysis tasks.
    *
    * @param root A shared pointer to the root of the abstract syntax tree (AST).
    * @param visitor A pointer to the ASTVisitor used for visiting nodes in the AST.
    * @return An integer status code indicating the success or failure of the extraction process.
  */
  int extractDesign(std::shared_ptr<TNode> root, ASTVisitor* visitor);
};

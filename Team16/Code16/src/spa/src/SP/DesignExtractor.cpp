#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

//#include "PKB.h"
#include "SP/DesignExtractor.h"
#include "TNode.h"

using namespace std;

std::string DesignExtractor::evaluateAssign(TNode &root) {
    // lhs is variable, rhs is expression
}

unordered_map<string, unordered_set<string>> DesignExtractor::populatePKB() {
    std::cout << "populating PKB " << std::endl;
    return assignVarHashmap;
}

void DesignExtractor::extractAllVariablesAndConstants(string lhs, TNode &node) {
    std::cout << "extracting all variables " << std::endl;
    if (node.type == "variable") {
        insertAssignVarHashmap(lhs, node.content);
        return;
    } else if (node.type == "constant") {
        insertAssignConstHashmap(lhs, node.content);
        return;
    } else if (node.type == "plus" || node.type == "minus") {
        std::cout << "extracting all lhs variables " << std::endl;
        extractAllVariablesAndConstants(lhs, node.children[0]);
        std::cout << "extracting all rhs variables " << std::endl;
        extractAllVariablesAndConstants(lhs, node.children[1]);
    }
    return;
}

int DesignExtractor::extractDesign(TNode &root) {
    std::cout << "extracting design from root " << root.type << std::endl;
    // check type of root
    if (root.type == "equal") {
        std::cout << "root is equal" << std::endl;
        extractAllVariablesAndConstants( root.children[0].content, root.children[1]);
    }
    return 0;
}

void DesignExtractor::insertAssignConstHashmap(string var, string expr) {
    std::cout << "inserting const " << expr << " into " << var << std::endl;
    unordered_set<std::string>& set = assignConstHashmap[var];
    set.insert(expr);
}

void DesignExtractor::insertAssignVarHashmap(string var, string expr) {
    std::cout << "inserting var " << expr << " into " << var << std::endl;
    unordered_set<std::string>& set = assignVarHashmap[var];
    set.insert(expr);
}

void DesignExtractor::insertVariablesHashmap(string var) {
    variablesHashmap.insert(var);
}

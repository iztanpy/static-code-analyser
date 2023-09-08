#ifndef SPA_DESIGNEXTRACTOR_H
#define SPA_DESIGNEXTRACTOR_H
#include <unordered_map>
#include <unordered_set>
#include <string>

using namespace std;

class TNode;

class DesignExtractor {
public:
    DesignExtractor() = default;
    unordered_map<string, unordered_set<string>> assignVarHashmap;
    unordered_map<string, unordered_set<string>> assignConstHashmap;
    unordered_set<string> variablesHashmap;
    unordered_set<string> constantsHashmap;

    int extractDesign(TNode& root);
    void extractAllVariablesAndConstants(string lhs, TNode& node);
    void insertAssignVarHashmap(string var, string expr);
    void insertAssignConstHashmap(string var, string expr);
    void insertVariablesHashmap(string var);
    unordered_map<string, unordered_set<string>> populatePKB();
    std::string evaluateAssign(TNode& root);
};

#endif //SPA_DESIGNEXTRACTOR_H

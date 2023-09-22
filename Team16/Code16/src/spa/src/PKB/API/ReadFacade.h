#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <utility>
#include "PKB/PKB.h"


typedef std::string partialMatch;
typedef std::string variable;
typedef int statementNumber;
typedef std::string constant;

class ReadFacade {
 private:
  PKB& pkb;

 public:
  explicit ReadFacade(PKB& pkb);

    std::unordered_set<statementNumber> getAllAssigns();

    std::unordered_set<std::pair<statementNumber, variable>, PairHash> getAssignPair(partialMatch partial);

    std::unordered_set<std::pair<statementNumber, variable>, PairHash> getAssignPair(Wildcard wildcard);

    std::unordered_set<statementNumber> getAssigns(Wildcard lhs, partialMatch rhs);

    std::unordered_set<statementNumber> getAssigns(Wildcard lhs, Wildcard rhs);

    std::unordered_set<statementNumber> getAssigns(partialMatch lhs, partialMatch rhs);

    std::unordered_set<statementNumber> getAssigns(partialMatch lhs, Wildcard rhs);

    // VariableStore methods

    std::unordered_set<variable> getVariables();

    // UsesStore methods

    bool isUses(statementNumber lineNumber, variable variableName);

    bool isUses(statementNumber lineNumber, Wildcard wildcard);

    std::unordered_set<variable> uses(statementNumber line);

    std::unordered_set<statementNumber> uses(StmtEntity type, variable variableName);

    std::unordered_set<statementNumber> uses(StmtEntity type, Wildcard wildcard);

    std::unordered_set<std::pair<statementNumber, variable>, PairHash> uses(StmtEntity type);

    // ModifiesStore methods

    bool isModifies(statementNumber lineNumber, variable variableName);

    bool isModifies(statementNumber lineNumber, Wildcard wildcard);

    std::unordered_set<variable> modifies(statementNumber line);

    std::unordered_set<statementNumber> modifies(StmtEntity type, variable variableName);

    std::unordered_set<statementNumber> modifies(StmtEntity type, Wildcard wildcard);

    std::unordered_set<std::pair<statementNumber, variable>, PairHash> modifies(StmtEntity type);

    // ConstantStore methods

    std::unordered_set<constant> getConstants();

    // StatementStore methods

    std::unordered_set<statementNumber> getStatements(StmtEntity type);

    // ParentStore methods

    std::unordered_set<statementNumber> parent(statementNumber statement, Wildcard wildcard);

    statementNumber parent(Wildcard wildcard, statementNumber statement);

    bool isParent(statementNumber parent, statementNumber child);

    bool isParent(statementNumber parent, Wildcard wildcard);

    bool isParent(Wildcard wildcard, statementNumber child);

    bool isParent(Wildcard wildcard, Wildcard wildcard2);

    std::unordered_set<statementNumber> parentStar(statementNumber statement, Wildcard wildcard);

    std::unordered_set<statementNumber> parentStar(Wildcard wildcard, statementNumber statement);

    bool isParentStar(statementNumber parent, statementNumber child);

    bool isParentStar(statementNumber parent, Wildcard wildcard);

    bool isParentStar(Wildcard wildcard, statementNumber child);

    bool isParentStar(Wildcard wildcard, Wildcard wildcard2);

    // FollowStore Method

    std::unordered_set<statementNumber> follows(Wildcard wildcard, StmtEntity entity);

    std::unordered_set<statementNumber> follows(statementNumber num, StmtEntity entity);

    std::unordered_set<statementNumber> follows(StmtEntity entity, statementNumber num);

    std::unordered_set<statementNumber> follows(StmtEntity entity, Wildcard wildcard);

    std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash>  follows(StmtEntity entity1, StmtEntity entity2);

    bool isFollow(statementNumber statement1, statementNumber statement2);

    bool isFollow(statementNumber statement1, Wildcard wildcard);

    bool isFollow(Wildcard wildcard, statementNumber statement1);

    bool isFollow(Wildcard wildcard, Wildcard wildcard2);

    std::unordered_set<statementNumber> followStar(Wildcard wildcard, StmtEntity entity);

    std::unordered_set<statementNumber> followStar(statementNumber num, StmtEntity entity);

    std::unordered_set<statementNumber> followStar(StmtEntity entity, statementNumber num);

    std::unordered_set<statementNumber> followStar(StmtEntity entity, Wildcard wildcard);

    bool isFollowStar(statementNumber statement1, statementNumber statement2);

    bool isFollowStar(statementNumber statement1, Wildcard wildcard);

    bool isFollowStar(Wildcard wildcard, statementNumber statement1);

    bool isFollowStar(Wildcard wildcard, Wildcard wildcard2);
};

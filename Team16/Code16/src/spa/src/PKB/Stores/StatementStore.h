#include <string>
#include <unordered_set>
#include <unordered_map>


class StatementStore {
 private:
    typedef std::string variable;
    typedef std::string constant;
    typedef int statementNumber;
    typedef std::string statementType;

    std::unordered_map<statementNumber, statementType> numTypeMap;


 public:
    StatementStore();

    void addStatement(std::unordered_map<statementNumber, statementType> typeMap);

    std::unordered_set<statementNumber> getStatements(statementType type);
};


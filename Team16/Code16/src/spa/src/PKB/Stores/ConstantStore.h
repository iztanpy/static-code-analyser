//
// Created by Isaac Tan on 12/9/23.
//

#include <string>
#include <unordered_set>


class ConstantStore {
 private:
    typedef std::string constant;
    std::unordered_set<constant> constants;

 public:
    ConstantStore();

    void addConstants(std::unordered_set<constant> constants);

    std::unordered_set<constant> getConstants();
};

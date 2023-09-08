#pragma once

#include <string>
#include <set>

enum class DesignEntity {
    STMT, READ, PRINT, CALL, WHILE_LOOP, IF_STMT, ASSIGN, VARIABLE, CONSTANT, PROCEDURE
};

class Entity {
 public:
    static std::string toString(DesignEntity entity);
    static DesignEntity fromString(const std::string& entity_str);
    static std::set<std::string> getStringDesignEntities();
};

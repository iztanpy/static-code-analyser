#pragma once

#include <string>

enum class DesignEntity {
  STMT, READ, PRINT, CALL, WHILE_LOOP, IF_STMT, ASSIGN, VARIABLE, CONSTANT, PROCEDURE
};

std::string toString(DesignEntity entity);
DesignEntity fromString(const std::string& entity_str);

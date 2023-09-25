#include <iostream>
#include <string>
#include <map>

#include "qps/design_entity.h"

// ai-gen start (gpt3, 1)
std::string Entity::toString(DesignEntity entity) {
  static const std::map<DesignEntity, std::string> entity_to_string = {
      {DesignEntity::STMT, "stmt"},
      {DesignEntity::READ, "READ"},
      {DesignEntity::PRINT, "print"},
      {DesignEntity::CALL, "call"},
      {DesignEntity::WHILE_LOOP, "while"},
      {DesignEntity::IF_STMT, "if"},
      {DesignEntity::ASSIGN, "assign"},
      {DesignEntity::VARIABLE, "variable"},
      {DesignEntity::CONSTANT, "constant"},
      {DesignEntity::PROCEDURE, "procedure"}
  };

  return entity_to_string.at(entity);
}

DesignEntity Entity::fromString(const std::string & entity_str) {
  static const std::map<std::string, DesignEntity> string_to_entity = {
      {"stmt", DesignEntity::STMT},
      {"read", DesignEntity::READ},
      {"print", DesignEntity::PRINT},
      {"call", DesignEntity::CALL},
      {"while", DesignEntity::WHILE_LOOP},
      {"if", DesignEntity::IF_STMT},
      {"assign", DesignEntity::ASSIGN},
      {"variable", DesignEntity::VARIABLE},
      {"constant", DesignEntity::CONSTANT},
      {"procedure", DesignEntity::PROCEDURE}
  };

  return string_to_entity.at(entity_str);
}
// ai-gen end

std::set<std::string> Entity::getStringDesignEntities() {
  return {"stmt", "read", "print", "while", "if", "assign",
          "variable", "constant", "procedure"};
}

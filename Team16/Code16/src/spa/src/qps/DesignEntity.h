//
// Created by Cuong Ngo on 7/9/23.
//
#pragma once

#include <string>

enum class DesignEntity {
  STMT, READ, PRINT, CALL, WHILE_LOOP, IF_STMT, ASSIGN, VARIABLE, CONSTANT, PROCEDURE
};

std::string designEntityToString(DesignEntity entity);
DesignEntity stringToDesignEntity(const std::string& entity_str);

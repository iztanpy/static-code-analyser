#pragma once

#include <stdio.h>
#include <variant>
#include <string>

#include "qps/qps_errors/qps_semantic_error.h"

/*!
 * An enum class that represents the different types of statements
 */
enum class StmtEntity {
  kStmt,
  kRead,
  kPrint,
  kCall,
  kWhile,
  kIf,
  kAssign,
  kProcedure
};

/*!
 * An enum class that represents the different types of design entities
 */
enum class DesignEntity {
  STMT, READ, PRINT, CALL, WHILE_LOOP, IF_STMT, ASSIGN, VARIABLE, CONSTANT, PROCEDURE
};

/*!
 * An enum class that represents "_"
 */
enum class Wildcard {
  Value,
};

/*!
 * An enum class that represents a Procedure declaration
 */
enum class Dec {
  Procedure,
};

/*!
 * Converts a DesignEntity to a StmtEntity
 * @param entity The DesignEntity to convert
 * @return The converted StmtEntity
 */
static StmtEntity ConvertToStmtEntity(DesignEntity entity) {
  switch (entity) {
    case DesignEntity::STMT: return StmtEntity::kStmt;
    case DesignEntity::READ: return StmtEntity::kRead;
    case DesignEntity::PRINT: return StmtEntity::kPrint;
    case DesignEntity::CALL: return StmtEntity::kCall;
    case DesignEntity::WHILE_LOOP: return StmtEntity::kWhile;
    case DesignEntity::IF_STMT: return StmtEntity::kIf;
    case DesignEntity::ASSIGN: return StmtEntity::kAssign;
    case DesignEntity::PROCEDURE: return StmtEntity::kProcedure;

    case DesignEntity::VARIABLE:
    case DesignEntity::CONSTANT: throw QpsSemanticError("Cannot convert this DesignEntity to StmtEntity");
  }
}

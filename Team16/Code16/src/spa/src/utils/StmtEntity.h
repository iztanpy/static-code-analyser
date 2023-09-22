#pragma once

#include <stdio.h>

#include "qps/qps_errors/qps_semantic_error.h"

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

enum class DesignEntity {
  STMT, READ, PRINT, CALL, WHILE_LOOP, IF_STMT, ASSIGN, VARIABLE, CONSTANT, PROCEDURE
};

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
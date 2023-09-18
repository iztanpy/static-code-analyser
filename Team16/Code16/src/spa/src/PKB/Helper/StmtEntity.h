#pragma once
#include <stdio.h>

enum class StmtEntity {
    kStmt,
    kRead,
    kPrint,
    kCall,
    kWhile,
    kIf,
    kAssign
};
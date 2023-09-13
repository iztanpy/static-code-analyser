//
// Created by Isaac Tan on 13/9/23.
//

#ifndef SPA_PKB_H
#define SPA_PKB_H

#include "PKB/Stores/AssignStore.h"
#include "PKB/Stores/ConstantStore.h"
#include "PKB/Stores/VariableStore.h"
#include "PKB/Stores/StatementStore.h"
#include "PKB/Stores/UsesStore.h"



class PKB {
private:
    AssignStore* assignStore = new AssignStore();
    ConstantStore* constantStore = new ConstantStore();
    VariableStore* variableStore = new VariableStore();
    StatementStore* statementStore = new StatementStore();
    UsesStore* usesStore = new UsesStore();
public:
    PKB();
    friend class ReadFacade;
    friend class WriteFacade;
};


#endif //SPA_PKB_H

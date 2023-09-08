#include "qps/select_clause.h"

bool SelectClause::equals(const SelectClause& other) const {
    return (this->design_entity == other.design_entity) && (this->synonym == other.synonym);
}


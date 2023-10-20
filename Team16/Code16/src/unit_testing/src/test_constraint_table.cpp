#include "catch.hpp"

#include "qps/query_evaluator/constraint_solver/constraint_table.h"

/*
 * Since the implementation of ConstraintTable makes heavy use of std::unordered_set
 * for performance reason. We can't really assert the order in which the row appear
 * in our table, making it hard to test. Implementing the ConstraintTable using
 * unordered_map of ColName to vector of Cell doesn't make easier for us to test
 * that the rows take valid values.
 *
 * The main test function is [REQUIRE_TABLE_CONTENT] is designed around this
 * - We test length of our table
 * - We test if we can find all rows in our final table.
 */

// TODO(phuccuongngo99): Maybe test to make sure no duplicate rows in the table

// Test length of table
void REQUIRE_LENGTH(const Table& table, int length) {
    for (const auto& [col_name, col_values] : table) {
        REQUIRE(col_values.size() == length);
    }
}

void REQUIRE_EXISTS(Table& table, const std::vector<ColName>& col_names, const std::vector<Cell>& row) {
    // Assert to make sure the programmer passes in the right parameter
    assert(col_names.size() == row.size());

    // Assert that std::vector<ColName> all exists as key in our table
    for (const auto& col_name : col_names) {
        assert(table.find(col_name) != table.end());
    }

    // Assert that table doesn't have any other columns
    assert(table.size() == col_names.size());

    bool row_exists = false;
    for (size_t row_ind = 0; row_ind < table[col_names[0]].size(); ++row_ind) {
        bool row_satisfy = true;
        for (size_t col_ind = 0; col_ind < col_names.size(); ++col_ind) {
            if (table[col_names[col_ind]][row_ind] != row[col_ind]) {
                row_satisfy = false;
                break;
            }
        }
        if (row_satisfy) {
            row_exists = true;
            break;
        }
    }
    REQUIRE(row_exists);
}

void REQUIRE_TABLE_CONTENT(Table& table,
                           const std::vector<ColName>& col_names,
                           const std::vector<std::vector<Cell>>& rows) {
    // Assert parameter
    assert(col_names.size() == rows[0].size());

    REQUIRE_LENGTH(table, rows.size());

    for (const auto& row : rows) {
        REQUIRE_EXISTS(table, col_names, row);
    }
}

TEST_CASE("ConstraintTable::HasNoValidValues", "[ConstraintTable]") {
    ConstraintTable ct;

    SECTION("Only True Constraint") {
        REQUIRE(ct.IsValid());

        Constraint constraint1 = true;
        ct.Solve(constraint1);
        // Only has True Constraint so far
        REQUIRE(ct.IsValid());

        Constraint constraint2 = true;
        ct.Solve(constraint2);
        // Only has True Constraint so far
        REQUIRE(ct.IsValid());

        Constraint constraint3 = UnaryConstraint{"a", {}};
        ct.Solve(constraint3);
        // Face non-bool constraint which is empty
        REQUIRE(!ct.IsValid());
    }

    SECTION("Face at least 1 false constraint") {
        REQUIRE(ct.IsValid());

        Constraint constraint1 = true;
        ct.Solve(constraint1);
        // Only has True Constraint so far
        REQUIRE(ct.IsValid());

        Constraint constraint2 = UnaryConstraint{"a", {"1", "2", "3"}};
        ct.Solve(constraint2);
        // Has a valid non-bool constraint that has some valid assignments
        REQUIRE(ct.IsValid());

        Constraint constraint3 = true;
        ct.Solve(constraint3);
        // A true constraint shouldn't affect anything
        REQUIRE(ct.IsValid());

        Constraint constraint4 = false;
        ct.Solve(constraint4);
        REQUIRE(!ct.IsValid());

        Constraint constraint5 = true;
        ct.Solve(constraint5);
        // Once it has been set to IsValid, it will not toggle back
        REQUIRE(!ct.IsValid());

        Constraint constraint6 = UnaryConstraint{"b", {"34", "42", "38"}};
        ct.Solve(constraint6);
        // Once it has been set to IsValid, it will not toggle back
        REQUIRE(!ct.IsValid());
    }
}

TEST_CASE("ConstraintTable::AddNewUnaryConstraint", "[ConstraintTable]") {

    SECTION("Adding a new unary constraint to new table") {
        ConstraintTable ct;

        Constraint constraint = UnaryConstraint{"c", {"4", "5", "6"}};
        ct.Solve(constraint);
        auto table = ct.GetTableForTesting();

        std::vector<ColName> col_names = {"c"};
        std::vector<std::vector<Cell>> rows = {{"4"}, {"5"}, {"6"}};
        REQUIRE_TABLE_CONTENT(table, col_names, rows);
        REQUIRE(ct.AvailableColName() == std::unordered_set<ColName>{col_names.begin(), col_names.end()});

        Constraint c = UnaryConstraint{"d", {}};
        ct.Solve(c);
    }

    SECTION("Adding a new unary constraint to existing table") {
        ConstraintTable ct = ConstraintTable::ForTestingOnly(
            {
                {"a", {"1", "2"}},
                {"b", {"3", "4"}},
            });

        Constraint constraint = UnaryConstraint{"c", {"4", "5"}};
        ct.Solve(constraint);
        auto table = ct.GetTableForTesting();

        std::vector<ColName> col_names = {"a", "b", "c"};
        std::vector<std::vector<Cell>> rows = {{"1", "3", "4"}, {"1", "3", "5"}, {"2", "4", "4"}, {"2", "4", "5"}};
        REQUIRE_TABLE_CONTENT(table, col_names, rows);
        REQUIRE(ct.AvailableColName() == std::unordered_set<ColName>{col_names.begin(), col_names.end()});
    }
}

TEST_CASE("ConstraintTable::AddNewBinaryConstraint", "[ConstraintTable]") {

    SECTION("Adding a new binary constraint to empty tables") {
        ConstraintTable ct;

        Constraint constraint = BinaryConstraint{{"c", "d"}, {{"6", "7"}, {"8", "9"}, {"10", "11"}}};
        ct.Solve(constraint);
        auto table = ct.GetTableForTesting();

        std::vector<ColName> col_names = {"c", "d"};
        std::vector<std::vector<Cell>> rows = {{"6", "7"}, {"8", "9"}, {"10", "11"}};
        REQUIRE_TABLE_CONTENT(table, col_names, rows);
        REQUIRE(ct.AvailableColName() == std::unordered_set<ColName>{col_names.begin(), col_names.end()});
    }

    SECTION("Adding a new binary constraint to existing tables") {
        ConstraintTable ct = ConstraintTable::ForTestingOnly(
            {
                {"a", {"1", "2"}},
                {"b", {"3", "4"}},
            });

        Constraint constraint = BinaryConstraint{{"c", "d"}, {{"6", "7"}, {"8", "9"}, {"10", "11"}}};
        ct.Solve(constraint);
        auto table = ct.GetTableForTesting();

        std::vector<ColName> col_names = {"a", "b", "c", "d"};
        std::vector<std::vector<Cell>> rows = {
            {"1", "3", "6", "7"},
            {"1", "3", "10", "11"},
            {"2", "4", "6", "7"},
            {"1", "3", "8", "9"},
            {"2", "4", "8", "9"},
            {"2", "4", "10", "11"},
        };
        REQUIRE_TABLE_CONTENT(table, col_names, rows);
        REQUIRE(ct.AvailableColName() == std::unordered_set<ColName>{col_names.begin(), col_names.end()});
    }
}

TEST_CASE("ConstraintTable::AddExistingUnaryConstraint", "[ConstraintTable]") {
    ConstraintTable ct = ConstraintTable::ForTestingOnly(
        {
            {"a", {"1", "2", "8", "9"}},
            {"b", {"3", "4", "10", "11"}},
            {"c", {"5", "6", "12", "13"}},
        }
    );

    SECTION("Adding a new unary constraint") {
        Constraint constraint_1 = UnaryConstraint{"a", {"1", "2", "9"}};
        Constraint constraint_2 = UnaryConstraint{"c", {"5", "13", "14", "1"}};
        std::vector<ColName> col_names = {"a", "b", "c"};

        ct.Solve(constraint_1);
        Table table1 = ct.GetTableForTesting();
        REQUIRE_TABLE_CONTENT(table1,
                              col_names,
                              {{"1", "3", "5"},
                               {"2", "4", "6"},
                               {"9", "11", "13"}});

        ct.Solve(constraint_2);
        Table table2 = ct.GetTableForTesting();
        REQUIRE_TABLE_CONTENT(table2,
                              col_names,
                              {{"1", "3", "5"},
                               {"9", "11", "13"}});
        REQUIRE(ct.AvailableColName() == std::unordered_set<ColName>{col_names.begin(), col_names.end()});
    };
}

TEST_CASE("ConstraintTable::AddExistingBinaryConstraint", "[ConstraintTable]") {
    ConstraintTable ct = ConstraintTable::ForTestingOnly(
        {
            {"a", {"1", "2", "8", "9"}},
            {"b", {"3", "4", "10", "11"}},
            {"c", {"5", "6", "12", "13"}},
        }
    );

    SECTION("Adding a new unary constraint") {
        Constraint constraint = BinaryConstraint{{"a", "b"}, {{"1", "3"}, {"2", "4"}, {"100", "11"}, {"9", "8"}}};
        ct.Solve(constraint);
        Table table = ct.GetTableForTesting();

        std::vector<ColName> col_names = {"a", "b", "c"};
        std::vector<std::vector<Cell>> rows = {{"1", "3", "5"}, {"2", "4", "6"}};
        REQUIRE_TABLE_CONTENT(table, col_names, rows);
        REQUIRE(ct.AvailableColName() == std::unordered_set<ColName>{col_names.begin(), col_names.end()});
    }
}

TEST_CASE("ConstraintTable::AddHalfExistingBinaryConstraint", "[ConstraintTable]") {
    ConstraintTable ct = ConstraintTable::ForTestingOnly(
        {
            {"a", {"1", "2", "8", "9"}},
            {"b", {"3", "4", "10", "11"}},
            {"c", {"5", "6", "12", "13"}},
        }
    );

    SECTION("Adding a new unary constraint") {
        Constraint constraint = BinaryConstraint{{"a", "d"}, {{"1", "2"}, {"1", "4"}, {"3", "11"}}};
        ct.Solve(constraint);
        Table table = ct.GetTableForTesting();

        std::vector<ColName> col_names = {"a", "b", "c", "d"};
        std::vector<std::vector<Cell>> rows = {{"1", "3", "5", "4"}, {"1", "3", "5", "2"}};
        REQUIRE_TABLE_CONTENT(table, col_names, rows);
        REQUIRE(ct.AvailableColName() == std::unordered_set<ColName>{col_names.begin(), col_names.end()});
    }
}

TEST_CASE("ConstraintTable::Select with Error when passing in non-existing ColName", "[ConstraintTable]") {
    ConstraintTable ct = ConstraintTable::ForTestingOnly(
        {
            {"a", {"1", "2", "2", "9"}},
            {"b", {"3", "4", "10", "11"}},
            {"c", {"5", "6", "12", "13"}},
        }
    );

    REQUIRE(ct.Select({"a"}) == std::unordered_set<std::string>{"1", "2", "9"});
    REQUIRE(ct.Select({"a", "a"}) == std::unordered_set<std::string>{"1 1", "2 2", "9 9"});
    REQUIRE(ct.Select({"b"}) == std::unordered_set<std::string>{"3", "4", "10", "11"});

    REQUIRE(ct.Select({"b", "a", "b"}) == std::unordered_set<std::string>{"3 1 3", "4 2 4", "10 2 10", "11 9 11"});

    Constraint constraint = BinaryConstraint{{"a", "d"}, {{"1", "2"}, {"1", "4"}, {"3", "11"}}};
    ct.Solve(constraint);

    REQUIRE(ct.Select({"a"}) == std::unordered_set<std::string>{"1"});
    REQUIRE(ct.Select({"b"}) == std::unordered_set<std::string>{"3"});
    REQUIRE(ct.Select({"c"}) == std::unordered_set<std::string>{"5"});
    REQUIRE(ct.Select({"d"}) == std::unordered_set<std::string>{"4", "2"});
}

TEST_CASE("ConstraintTable::JoinTable", "[ConstraintTable]") {
    // Initializing the main table
    ConstraintTable mainTable = ConstraintTable::ForTestingOnly(
        {
            {"a", {"1", "2"}},
            {"b", {"3", "4"}},
            {"c", {"5", "6"}},
        }
    );

    // Initializing the table to be joined
    ConstraintTable joinTable = ConstraintTable::ForTestingOnly(
        {
            {"d", {"7", "8", "9"}},
            {"e", {"10", "11", "12"}},
        }
    );

    // Joining the two tables
    mainTable.JoinTable(joinTable);
    Table table = mainTable.GetTableForTesting();

    SECTION("Checking merged content after joining") {
        std::vector<ColName> col_names = {"a", "b", "c", "d", "e"};  // Includes columns from both tables
        std::vector<std::vector<Cell>> rows = {
            {"1", "3", "5", "7", "10"},
            {"2", "4", "6", "8", "11"},
            {"1", "3", "5", "8", "11"},
            {"2", "4", "6", "7", "10"},
            {"1", "3", "5", "9", "12"},
            {"2", "4", "6", "9", "12"},
        };
        REQUIRE_TABLE_CONTENT(table, col_names, rows);
        REQUIRE(mainTable.AvailableColName() == std::unordered_set<ColName>{col_names.begin(), col_names.end()});
    }
}

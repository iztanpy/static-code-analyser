#include <memory>
#include <unordered_set>
#include <string>
#include "SP/tnode.h"
#include "catch.hpp"
#include "SP/source_processor.h"
#include "PKB/API/write_facade.h"
#include "PKB/API/read_facade.h"
#include "qps/qps.h"

TEST_CASE("One print statement 1") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    write_facade writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    QPS qps(readFacade);

    // Initialize SP and SP tokenizer
    std::string simpleProgram = "procedure k {print k;}";
    std::string query_1 = "variable v; Select v";
    sourceProcessor.processSource(simpleProgram);

    std::unordered_set<std::string> varSet = std::unordered_set<std::string>({ "k" });
    std::unordered_map<int, std::unordered_set<std::string>> usesLineVariable =
        std::unordered_map<int, std::unordered_set<std::string>>({ {1, {"k"}} });
    REQUIRE(sourceProcessor.getVariables() == varSet);
    REQUIRE(sourceProcessor.getUsesLineRHSVarMap() == usesLineVariable);
    REQUIRE(qps.Evaluate(query_1) == std::unordered_set<std::string>({ "k" }));

}

TEST_CASE("One read statement 1") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    write_facade writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    QPS qps(readFacade);

    // Initialize SP and SP tokenizer
    std::string simpleProgram = "procedure poo{read k;}";
    std::string query_1 = "variable v; Select v";
    sourceProcessor.processSource(simpleProgram);
    REQUIRE(qps.Evaluate(query_1) == std::unordered_set<std::string>({ "k" }));

    std::unordered_set<std::string> varSet = std::unordered_set<std::string>({ "k" });
    std::unordered_map<int, std::unordered_set<std::string>>
        modifiesLineVariable = std::unordered_map<int, std::unordered_set<std::string>>({ {1, {"k"}} });
    REQUIRE(sourceProcessor.getVariables() == varSet);
}

TEST_CASE("TEST SP-PKB-QPS UsesP ModifiesP") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    write_facade writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    QPS qps(readFacade);
    std::string simpleProgram = "procedure poo { y = x + 1; read k; print l; }";
    std::string query_1 = "variable v; procedure p; Select v such that Uses(p, v)";
    sourceProcessor.processSource(simpleProgram);
    REQUIRE((readFacade.isUses("poo", "x")));
    REQUIRE((readFacade.isUses("poo", "l")));
    REQUIRE((readFacade.isModifies("poo", "y")));
    REQUIRE((readFacade.isModifies("poo", "k")));
}

TEST_CASE("TEST SP-PKB Connection 2") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    write_facade writeFacade = write_facade(*pkb_ptr);

    writeFacade.storeCalls({ {"main", {"p", "q"}}, {"p", {"q"}}, });

    writeFacade.storeProcedures({ "main", "p", "q" });

    std::unordered_map<procedure, std::pair<int, int>> procedures = { {"main", {1, 4}}, {"p", {5, 8}}, {"q", {9, 10}} };

    writeFacade.storeUses({ {1, {"x", "y"}}, {2, {"y", "z"}}, {5, {"a"}}, {6, {"b"}}, {7, {"c"}}, {9, {"m"}} });

    writeFacade.storeModifies({ {1, {"x"}}, {2, {"y"}}, {3, {"z"}}, {5, {"a"}}, {6, {"b"}}, {7, {"c"}}, {9, {"m"}} });

    writeFacade.storeProcedures(procedures);

    REQUIRE((readFacade.isModifies("main", "x")));
    REQUIRE((readFacade.isModifies("main", "y")));
    REQUIRE((readFacade.isModifies("main", "z")));
    REQUIRE((readFacade.isModifies("main", "a")));
    REQUIRE((readFacade.isModifies("main", "b")));
    REQUIRE((readFacade.isModifies("main", "c")));
    REQUIRE((readFacade.isModifies("main", "m")));

    REQUIRE((readFacade.isUses("main", "x")));
    REQUIRE((readFacade.isUses("main", "y")));
    REQUIRE((readFacade.isUses("main", "z")));
    REQUIRE((readFacade.isUses("main", "a")));
    REQUIRE((readFacade.isUses("main", "b")));
    REQUIRE((readFacade.isUses("main", "c")));
    REQUIRE((readFacade.isUses("main", "m")));

    Wildcard w = Wildcard();

    REQUIRE((readFacade.isModifies("main", w)));
    REQUIRE((readFacade.isModifies("p", w)));
    REQUIRE((readFacade.isModifies("q", w)));

    REQUIRE((readFacade.isUses("main", w)));
    REQUIRE((readFacade.isUses("p", w)));
    REQUIRE((readFacade.isUses("q", w)));
    REQUIRE((!readFacade.isUses("r", w)));

    REQUIRE((!readFacade.isModifies("p", "x")));
    REQUIRE((!readFacade.isModifies("p", "y")));
    REQUIRE((!readFacade.isModifies("p", "z")));
    REQUIRE((readFacade.isModifies("p", "a")));
    REQUIRE((readFacade.isModifies("p", "b")));
    REQUIRE((readFacade.isModifies("p", "c")));
    REQUIRE((readFacade.isModifies("p", "m")));

    REQUIRE((!readFacade.isUses("p", "x")));
    REQUIRE((!readFacade.isUses("p", "y")));
    REQUIRE((!readFacade.isUses("p", "z")));
    REQUIRE((readFacade.isUses("p", "a")));
    REQUIRE((readFacade.isUses("p", "b")));
    REQUIRE((readFacade.isUses("p", "c")));
    REQUIRE((readFacade.isUses("p", "m")));

    REQUIRE((!readFacade.isModifies("q", "x")));
    REQUIRE((!readFacade.isModifies("q", "y")));
    REQUIRE((!readFacade.isModifies("q", "z")));
    REQUIRE((!readFacade.isModifies("q", "a")));
    REQUIRE((!readFacade.isModifies("q", "b")));
    REQUIRE((!readFacade.isModifies("q", "c")));
    REQUIRE((readFacade.isModifies("q", "m")));

    REQUIRE((!readFacade.isUses("q", "x")));
    REQUIRE((!readFacade.isUses("q", "y")));
    REQUIRE((!readFacade.isUses("q", "z")));
    REQUIRE((!readFacade.isUses("q", "a")));
    REQUIRE((!readFacade.isUses("q", "b")));
    REQUIRE((!readFacade.isUses("q", "c")));
    REQUIRE((readFacade.isUses("q", "m")));

    REQUIRE((readFacade.modifies("main") == std::unordered_set<variable>({ "x", "y", "z", "a", "b", "c", "m" })));
    REQUIRE((readFacade.modifies("p") == std::unordered_set<variable>({ "a", "b", "c", "m" })));
    REQUIRE((readFacade.modifies("q") == std::unordered_set<variable>({ "m" })));

    REQUIRE((readFacade.uses("main") == std::unordered_set<variable>({ "x", "y", "z", "a", "b", "c", "m" })));
    REQUIRE((readFacade.uses("p") == std::unordered_set<variable>({ "a", "b", "c", "m" })));
    REQUIRE((readFacade.uses("q") == std::unordered_set<variable>({ "m" })));

    REQUIRE((readFacade.modifiesProcedure(w) == std::unordered_set<procedure>({ "main", "p", "q" })));
    REQUIRE((readFacade.usesProcedure(w) == std::unordered_set<procedure>({ "main", "p", "q" })));

    REQUIRE((readFacade.modifiesProcedure("x") == std::unordered_set<procedure>({ "main" })));
    REQUIRE((readFacade.usesProcedure("x") == std::unordered_set<procedure>({ "main" })));

    REQUIRE((readFacade.modifiesProcedure("a") == std::unordered_set<procedure>({ "main", "p" })));
    REQUIRE((readFacade.usesProcedure("a") == std::unordered_set<procedure>({ "main", "p" })));

    REQUIRE((readFacade.modifiesProcedure("m") == std::unordered_set<procedure>({ "main", "p", "q" })));
    REQUIRE((readFacade.usesProcedure("m") == std::unordered_set<procedure>({ "main", "p", "q" })));
}

TEST_CASE("Calls and Callstar methods") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    write_facade writeFacade = write_facade(*pkb_ptr);

    writeFacade.storeCalls({ {"main", {"p"}}, {"p", {"q"}}, });

    writeFacade.storeProcedures({ "main", "p", "q" });

    REQUIRE(readFacade.isCall("main", "p"));
    REQUIRE(!readFacade.isCall("main", "q"));
    REQUIRE(readFacade.isCall("p", "q"));
    REQUIRE(!readFacade.isCall("p", "main"));
    REQUIRE(!readFacade.isCall("q", "main"));
    REQUIRE(!readFacade.isCall("q", "p"));

    StmtEntity dec1 = StmtEntity::kProcedure;
    StmtEntity dec2 = StmtEntity::kProcedure;

    Wildcard w = Wildcard();

    REQUIRE(readFacade.isCall(w, w));
    REQUIRE(readFacade.isCall("main", w));
    REQUIRE(readFacade.isCall(w, "p"));
    REQUIRE(readFacade.isCall("p", w));
    REQUIRE(readFacade.isCall("main", "p"));
    REQUIRE(!readFacade.isCall("q", w));
    REQUIRE(readFacade.isCall(w, "q"));
    REQUIRE(!readFacade.isCall(w, "main"));
    REQUIRE(!readFacade.isCall("q", "p"));
    REQUIRE(readFacade.isCall("p", "q"));

    REQUIRE(readFacade.isCallStar(w, w));
    REQUIRE(readFacade.isCallStar("main", w));
    REQUIRE(readFacade.isCallStar(w, "p"));
    REQUIRE(readFacade.isCallStar("p", w));
    REQUIRE(readFacade.isCallStar("main", "p"));
    REQUIRE(readFacade.isCallStar("main", "q"));
    REQUIRE(readFacade.isCallStar("p", "q"));
    REQUIRE(!readFacade.isCallStar("q", w));
    REQUIRE(readFacade.isCallStar(w, "q"));
    REQUIRE(!readFacade.isCallStar(w, "main"));
    REQUIRE(!readFacade.isCallStar("q", "p"));

    REQUIRE((readFacade.call(dec1, w) == std::unordered_set<procedure>({ "p", "main" })));
    REQUIRE((readFacade.callStar(dec1, w) == std::unordered_set<procedure>({ "p", "main" })));

    REQUIRE((readFacade.call(w, dec1) == std::unordered_set<procedure>({ "p", "q" })));
    REQUIRE((readFacade.callStar(w, dec1) == std::unordered_set<procedure>({ "p", "q" })));

    REQUIRE((readFacade.call(dec1, dec2)
        == std::unordered_set<std::pair<procedure, procedure>, PairHash>({ {"main", "p"}, {"p", "q"} })));
    REQUIRE((readFacade.callStar(dec1, dec2)
        == std::unordered_set<std::pair<procedure, procedure>, PairHash>({ {"main", "p"}, {"p", "q"}, {"main", "q"} })));

    REQUIRE((readFacade.call(dec1, "p") == std::unordered_set<procedure>({ "main" })));
    REQUIRE((readFacade.callStar(dec1, "p") == std::unordered_set<procedure>({ "main"})));
    
    REQUIRE((readFacade.call("p", dec2) == std::unordered_set<procedure>({ "q" })));
    REQUIRE((readFacade.callStar("p", dec2) == std::unordered_set<procedure>({ "q" })));

    REQUIRE((readFacade.callStar("main", dec2) == std::unordered_set<procedure>({ "p", "q" })));
}

TEST_CASE("Calls and Callstar methods error") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    write_facade writeFacade = write_facade(*pkb_ptr);

    REQUIRE_THROWS(writeFacade.storeCalls({ {"main", {"p"}}, {"p", {"main"}}}));
    REQUIRE_THROWS(writeFacade.storeCalls({ {"main", {"p"}}, {"p", {"q"}}, {"q", {"main"}}}));
}

TEST_CASE("Test Calls ") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    write_facade writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    QPS qps(readFacade);

    // Initialize SP and SP tokenizer
    std::string simpleProgram = "procedure p {x = x + 1; y = y  + 2; call q;} procedure q {m = m + 1; call r;} procedure r { j = j + 1;}";

    sourceProcessor.processSource(simpleProgram);
    REQUIRE(readFacade.isModifies(3, "m"));
    REQUIRE(readFacade.isModifies(3, "j"));
    REQUIRE(readFacade.isModifies(5, "j"));
    REQUIRE(readFacade.isModifies(6, "j"));
    StmtEntity call = StmtEntity::kCall;
    REQUIRE(readFacade.modifies(call, "j") == std::unordered_set<statementNumber>({ 3, 5}));
    REQUIRE(pkb_ptr->getStatements(StmtEntity::kStmt) == std::unordered_set<statementNumber>({ 1, 2, 3, 4, 5, 6 }));
    REQUIRE(pkb_ptr->getStatements(call) == std::unordered_set<statementNumber>({ 3, 5}));
    REQUIRE(readFacade.modifies(StmtEntity::kStmt, "j") == std::unordered_set<statementNumber>({ 3, 5, 6 }));

}

TEST_CASE("Test SP-PKB connection") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    write_facade writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);

    std::string simpleProgram = "procedure p {x = z - 3 + I; x = x + 1; y = y + z + 4;}";

    sourceProcessor.processSource(simpleProgram);

    REQUIRE((readFacade.getAllAssigns() == std::unordered_set<statementNumber>({ 1, 2, 3 })));

    REQUIRE((readFacade.getAssignPair(PartialExpr{"(x)"})
        == std::unordered_set<std::pair<statementNumber, variable>, PairHash>({{2, "x"}})));

    REQUIRE((readFacade.getAssignPair(PartialExpr{"(y)"})
        == std::unordered_set<std::pair<statementNumber, variable>, PairHash>({{3, "y"}})));

    REQUIRE((readFacade.getAssignPair(PartialExpr{"(z)"})
        == std::unordered_set<std::pair<statementNumber, variable>, PairHash>({{1, "x"}, {3, "y"}})));

    Wildcard wildcard = Wildcard();

    REQUIRE((readFacade.getAssignPair(wildcard)
        == std::unordered_set<std::pair<statementNumber, variable>, PairHash>({ {1, "x"}, {2, "x"}, {3, "y"} })));

    REQUIRE((readFacade.getAssigns(wildcard, PartialExpr{"(x)"}) == std::unordered_set<statementNumber>({2})));

    REQUIRE((readFacade.getAssigns(wildcard, PartialExpr{"(y)"}) == std::unordered_set<statementNumber>({3})));

    REQUIRE((readFacade.getAssigns(wildcard, PartialExpr{"(z)"}) == std::unordered_set<statementNumber>({1, 3})));

    REQUIRE((readFacade.getAssigns(wildcard, wildcard) == std::unordered_set<statementNumber>({ 1, 2, 3 })));

    REQUIRE((readFacade.getAssigns("x", PartialExpr{"(z)"}) == std::unordered_set<statementNumber>({1})));

    REQUIRE((readFacade.getAssigns("x", wildcard) == std::unordered_set<statementNumber>({ 1, 2 })));

    REQUIRE((readFacade.isUses(1, "z")));

    REQUIRE((readFacade.isUses(1, "I")));

    REQUIRE(!(readFacade.isUses(1, "x")));

    REQUIRE((readFacade.isUses(2, "x")));

    REQUIRE((readFacade.isUses(3, "y")));

    REQUIRE((readFacade.isUses(3, "z")));

    REQUIRE((readFacade.uses(StmtEntity::kAssign, "z") == std::unordered_set<statementNumber>({ 1, 3 })));

    REQUIRE((readFacade.uses(StmtEntity::kAssign, "x") == std::unordered_set<statementNumber>({ 2 })));

    REQUIRE((readFacade.uses(StmtEntity::kAssign, "y") == std::unordered_set<statementNumber>({ 3 })));

    REQUIRE((readFacade.uses(StmtEntity::kAssign, wildcard) == std::unordered_set<statementNumber>({ 1, 2, 3 })));

    std::unordered_set<std::pair<statementNumber, variable>, PairHash> pairSet =
        std::unordered_set<std::pair<statementNumber, variable>, PairHash>({ {1, "z"}, {1, "I"}, {2, "x"}, {3, "y"},
                                                                            {3, "z"} });

    REQUIRE((readFacade.uses(StmtEntity::kAssign) == pairSet));

    REQUIRE(readFacade.getVariables() == std::unordered_set<variable>({ "x", "z", "I", "y" }));

    REQUIRE(readFacade.modifies(1) == std::unordered_set<variable>({ "x" }));
    REQUIRE(readFacade.modifies(2) == std::unordered_set<variable>({ "x" }));
    REQUIRE(readFacade.modifies(3) == std::unordered_set<variable>({ "y" }));

    REQUIRE(readFacade.uses(1) == std::unordered_set<variable>({ "z", "I" }));
    REQUIRE(readFacade.uses(2) == std::unordered_set<variable>({ "x" }));
    REQUIRE(readFacade.uses(3) == std::unordered_set<variable>({ "y", "z" }));
}

TEST_CASE("One assign statement 1") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    write_facade writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    QPS qps(readFacade);

    // Initialize SP and SP tokenizer
    std::string simpleProgram = "procedure p {x = x + 1;}";
    std::string query_1 = "variable v; Select v";
    std::string query_2 = "constant c; Select c";

    sourceProcessor.processSource(simpleProgram);

    std::unordered_set<std::string> varSet = std::unordered_set<std::string>({"x"});
    std::unordered_map<std::string, std::unordered_set<std::string>>
        varUseMap = std::unordered_map<std::string, std::unordered_set<std::string>>(
        {{"x", varSet}});
    std::unordered_set<std::string> constSet = std::unordered_set<std::string>({"1"});
    std::unordered_map<std::string, std::unordered_set<std::string>>
        constUseMap = std::unordered_map<std::string, std::unordered_set<std::string>>(
        {{"x", constSet}});
    std::unordered_map<int, std::unordered_set<std::string>>
        assignLinePartialRHSPatternMap = std::unordered_map<int, std::unordered_set<std::string>>({{1, {"(x)", "(1)", "((x)+(1))"}}});
    std::unordered_map<int, std::string> usesLineLHSMap = std::unordered_map<int, std::string>({{1, "x"}});
    std::unordered_map<int, std::unordered_set<std::string>>
        usesLineRHSVarMap = std::unordered_map<int, std::unordered_set<std::string>>({{1, {"x"}}});

    REQUIRE(sourceProcessor.getAssignLinePartialRHSPatternMap() == assignLinePartialRHSPatternMap);
    REQUIRE(sourceProcessor.getUsesLineLHSMap() == usesLineLHSMap);
    REQUIRE(sourceProcessor.getUsesLineRHSVarMap() == usesLineRHSVarMap);
    REQUIRE(sourceProcessor.getVariables() == varSet);
    REQUIRE(sourceProcessor.getConstants() == constSet);

    REQUIRE(qps.Evaluate(query_1) == std::unordered_set<std::string>({ "x" }));
    REQUIRE(qps.Evaluate(query_2) == std::unordered_set<std::string>({ "1" }));
}

TEST_CASE("One assign statement with white-spaces") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    write_facade writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    QPS qps(readFacade);

    std::string simpleProgram = "procedure test { \n x = \t \n \n\t y + \t 1; \n }";
    std::string query_1 = "variable v\t\t\n; Select \n v";
    std::string query_2 = "\n\t constant \t c; Select c";

    sourceProcessor.processSource(simpleProgram);

    REQUIRE(qps.Evaluate(query_1) == std::unordered_set<std::string>({ "x", "y" }));
    REQUIRE(qps.Evaluate(query_2) == std::unordered_set<std::string>({ "1" }));
}

TEST_CASE("Simple assign statements") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    write_facade writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    QPS qps(readFacade);

    std::string simpleProgram = "procedure p {x = 3; y = 4;}";
    std::string query_1 = "variable v; Select v";
    std::string query_2 = "constant c; Select c";

    sourceProcessor.processSource(simpleProgram);

    REQUIRE(qps.Evaluate(query_1) == std::unordered_set<std::string>({ "x", "y" }));
    REQUIRE(qps.Evaluate(query_2) == std::unordered_set<std::string>({ "3", "4" }));
}

TEST_CASE("Multiple assign statements") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    write_facade writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    QPS qps(readFacade);

    std::string simpleProgram = "procedure p{x = z - 3; y = y + 4; i = i + 10;}";
    std::string query_1 = "variable v; Select v";
    std::string query_2 = "constant c; Select c";

    sourceProcessor.processSource(simpleProgram);

    REQUIRE(qps.Evaluate(query_1) == std::unordered_set<std::string>({ "x", "y", "z", "i" }));
    REQUIRE(qps.Evaluate(query_2) == std::unordered_set<std::string>({ "3", "4", "10" }));
}

TEST_CASE("Assign statements with many SIMPLE RHS terms and whitespaces") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    write_facade writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    QPS qps(readFacade);
    std::string simpleProgram = "procedure p{x = z - 3 + I - \n 100 + \t u100 + U48ka - \n \t OoOhd; \t  y = y + 4; }";
    std::string query_1 = "variable v; Select v";
    std::string query_2 = "constant c; Select c";

    sourceProcessor.processSource(simpleProgram);

    REQUIRE(qps.Evaluate(query_1)
        == std::unordered_set<std::string>({ "x", "y", "z", "I", "u100", "U48ka", "OoOhd" }));
    REQUIRE(qps.Evaluate(query_2) == std::unordered_set<std::string>({ "3", "4", "100" }));
}

TEST_CASE("Assign statements with mixed-case PQL synonyms & many declarations") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    write_facade writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    QPS qps(readFacade);

    std::string
        simpleProgram = "procedure c {x = z - 3 + I - \n 100 + \t u100 + U48ka - \n \t OoOhd;} procedure procedure "
        "{ \t  y = y + 4;}";
    std::string query_1 = "variable Vj5u, v39; constant OvO; Select Vj5u";
    std::string query_2 = "constant c10ueYwh; variable Ur; Select c10ueYwh";

    sourceProcessor.processSource(simpleProgram);
    REQUIRE(qps.Evaluate(query_1)
        == std::unordered_set<std::string>({ "x", "y", "z", "I", "u100", "U48ka", "OoOhd" }));
    REQUIRE(qps.Evaluate(query_2) == std::unordered_set<std::string>({ "3", "4", "100" }));
}

TEST_CASE("Selecting Assign statements") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    write_facade writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    QPS qps(readFacade);

    std::string
        simpleProgram = "procedure c {x = z - 3 + I - \n 100 + \t u100 + U48ka - \n \t OoOhd;} procedure procedure "
        "{ \t  read r; y = y + 4;}";
    std::string query_1 = "assign a; Select a";

    sourceProcessor.processSource(simpleProgram);
    REQUIRE(qps.Evaluate(query_1) == std::unordered_set<std::string>({ "1", "3" }));
}


TEST_CASE("Test Call Store functionalityies") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    write_facade writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    QPS qps(readFacade);

    std::string simpleProgram = R"(procedure a {
                                      call b;
                                      cc = dd + ee; }

                                    procedure b {
                                      read xx;
                                      call c; }

                                    procedure c {
                                      print bb;
                                      print cc; })";


    sourceProcessor.processSource(simpleProgram);
    REQUIRE((readFacade.isCall("a", "b")));
    REQUIRE((readFacade.isCall("b", "c")));
    REQUIRE((!readFacade.isCall("a", "c")));

    std::unordered_set<std::pair<statementNumber, procedure>, PairHash> callPairs = { {1, "b"}, {4, "c"} };
        
    REQUIRE(pkb_ptr->getCallPairs() == callPairs);
    REQUIRE(readFacade.getStatementsAndVariable(StmtEntity::kCall) == callPairs);
    REQUIRE(readFacade.getStatementsAndVariable(StmtEntity::kPrint) == std::unordered_set<std::pair<statementNumber, variable>, PairHash>({ {5, "bb"}, {6, "cc"} }));
    REQUIRE(readFacade.getStatementsAndVariable(StmtEntity::kRead) == std::unordered_set<std::pair<statementNumber, variable>, PairHash>({ {3, "xx"} }));
    REQUIRE(readFacade.getStatementsAndVariable(StmtEntity::kAssign) == std::unordered_set<std::pair<statementNumber, variable>, PairHash>({}));

}

TEST_CASE("Test affects testcase") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    write_facade writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    QPS qps(readFacade);
    std::string simpleProgram = R"(procedure Second {
        x = 0;
        i = 5;
        while (i!=0) {
            x = x + 2*y;
            call Third;
            i = i - 1; }
        if (x==1) then {
            x = x+1; }
          else {
            z = 1; }
        z = z + x + i;
        y = z + 2;
        x = x * y + z; }

      procedure Third {
          z = 5;
          v = z;
          print v; })";

    sourceProcessor.processSource(simpleProgram);
    REQUIRE(readFacade.isAffects(2, 6));
    REQUIRE(readFacade.isAffects(4, 8));
    REQUIRE(readFacade.isAffects(4, 10));
    REQUIRE(readFacade.isAffects(6, 6));
    REQUIRE(readFacade.isAffects(1, 4));
    REQUIRE(readFacade.isAffects(1, 8));
    REQUIRE(readFacade.isAffects(1, 10));
    REQUIRE(readFacade.isAffects(1, 12));
    REQUIRE(readFacade.isAffects(2, 10));
    REQUIRE(readFacade.isAffects(9, 10));
    REQUIRE(!readFacade.isAffects(9, 11));
    REQUIRE(!readFacade.isAffects(2, 3));
    REQUIRE(!readFacade.isAffects(9, 6));
    REQUIRE(!readFacade.isAffects(9, 12));
    REQUIRE(!readFacade.isAffects(8, 8));

    REQUIRE(!readFacade.isAffects(3, Wildcard()));
    REQUIRE(!readFacade.isAffects(Wildcard(), 3));

    REQUIRE(readFacade.isAffects(Wildcard(), 6));
    REQUIRE(readFacade.isAffects(1, Wildcard()));

    REQUIRE(readFacade.isAffects(Wildcard(), Wildcard()));

    REQUIRE(readFacade.isAffects(1, Wildcard()));
    REQUIRE(readFacade.isAffects(2, Wildcard()));
    REQUIRE(!readFacade.isAffects(3, Wildcard()));
    REQUIRE(readFacade.isAffects(4, Wildcard()));
    REQUIRE(!readFacade.isAffects(5, Wildcard()));
    REQUIRE(readFacade.isAffects(6, Wildcard()));
    REQUIRE(!readFacade.isAffects(7, Wildcard()));
    REQUIRE(readFacade.isAffects(8, Wildcard()));
    REQUIRE(readFacade.isAffects(9, Wildcard()));
    REQUIRE(readFacade.isAffects(10, Wildcard()));
    REQUIRE(readFacade.isAffects(11, Wildcard()));
    REQUIRE(readFacade.isAffects(1, Wildcard()));
    REQUIRE(!readFacade.isAffects(12, Wildcard()));
    REQUIRE(readFacade.isAffects(13, Wildcard()));
    REQUIRE(!readFacade.isAffects(51, Wildcard()));

    REQUIRE(!readFacade.isAffects(Wildcard(), 1));
    REQUIRE(!readFacade.isAffects(Wildcard(), 2));
    REQUIRE(!readFacade.isAffects(Wildcard(), 3));
    REQUIRE(readFacade.isAffects(Wildcard(), 4));
    REQUIRE(!readFacade.isAffects(Wildcard(), 5));
    REQUIRE(readFacade.isAffects(Wildcard(), 6));
    REQUIRE(!readFacade.isAffects(Wildcard(), 7));
    REQUIRE(readFacade.isAffects(Wildcard(), 8));
    REQUIRE(!readFacade.isAffects(Wildcard(), 9));
    REQUIRE(readFacade.isAffects(Wildcard(), 10));
    REQUIRE(readFacade.isAffects(Wildcard(), 11));
    REQUIRE(readFacade.isAffects(Wildcard(), 12));
    REQUIRE(!readFacade.isAffects(Wildcard(), 13));
    REQUIRE(readFacade.isAffects(Wildcard(), 14));
    REQUIRE(!readFacade.isAffects(Wildcard(), 51));

    REQUIRE(readFacade.Affects(1, StmtEntity::kAssign) == std::unordered_set<statementNumber>({ 4, 8, 10, 12 }));
    REQUIRE(readFacade.Affects(2, StmtEntity::kAssign) == std::unordered_set<statementNumber>({ 6, 10 }));
    REQUIRE(readFacade.Affects(3, StmtEntity::kAssign) == std::unordered_set<statementNumber>({}));
    REQUIRE(readFacade.Affects(4, StmtEntity::kAssign) == std::unordered_set<statementNumber>({ 4, 8, 10, 12 }));
    REQUIRE(readFacade.Affects(5, StmtEntity::kAssign) == std::unordered_set<statementNumber>({}));
    REQUIRE(readFacade.Affects(6, StmtEntity::kAssign) == std::unordered_set<statementNumber>({ 6, 10 }));
    REQUIRE(readFacade.Affects(7, StmtEntity::kAssign) == std::unordered_set<statementNumber>({}));
    REQUIRE(readFacade.Affects(8, StmtEntity::kAssign) == std::unordered_set<statementNumber>({ 10, 12 }));
    REQUIRE(readFacade.Affects(9, StmtEntity::kAssign) == std::unordered_set<statementNumber>({ 10 }));
    REQUIRE(readFacade.Affects(10, StmtEntity::kAssign) == std::unordered_set<statementNumber>({ 11, 12 }));
    REQUIRE(readFacade.Affects(11, StmtEntity::kAssign) == std::unordered_set<statementNumber>({ 12 }));
    REQUIRE(readFacade.Affects(12, StmtEntity::kAssign) == std::unordered_set<statementNumber>({}));
    REQUIRE(readFacade.Affects(13, StmtEntity::kAssign) == std::unordered_set<statementNumber>({ 14 }));
    REQUIRE(readFacade.Affects(14, StmtEntity::kAssign) == std::unordered_set<statementNumber>({}));
    REQUIRE(readFacade.Affects(51, StmtEntity::kAssign) == std::unordered_set<statementNumber>({}));


    pkb_ptr->clearAffectsCache();

    REQUIRE(readFacade.Affects(Wildcard(), StmtEntity::kAssign) == std::unordered_set<statementNumber>({ 4, 6, 8, 10, 11, 12, 14 }));
    REQUIRE(readFacade.Affects(StmtEntity::kAssign, Wildcard()) == std::unordered_set<statementNumber>({ 1, 2, 4, 6, 8, 9, 10, 11, 13 }));

    REQUIRE(readFacade.Affects(StmtEntity::kAssign, 1) == std::unordered_set<statementNumber>({}));
    REQUIRE(readFacade.Affects(StmtEntity::kAssign, 2) == std::unordered_set<statementNumber>({}));
    REQUIRE(readFacade.Affects(StmtEntity::kAssign, 3) == std::unordered_set<statementNumber>({}));
    REQUIRE(readFacade.Affects(StmtEntity::kAssign, 4) == std::unordered_set<statementNumber>({ 1, 4 }));
    REQUIRE(readFacade.Affects(StmtEntity::kAssign, 5) == std::unordered_set<statementNumber>({}));
    REQUIRE(readFacade.Affects(StmtEntity::kAssign, 6) == std::unordered_set<statementNumber>({ 2, 6 }));
    REQUIRE(readFacade.Affects(StmtEntity::kAssign, 7) == std::unordered_set<statementNumber>({}));
    REQUIRE(readFacade.Affects(StmtEntity::kAssign, 8) == std::unordered_set<statementNumber>({ 1, 4 }));
    REQUIRE(readFacade.Affects(StmtEntity::kAssign, 9) == std::unordered_set<statementNumber>({}));
    REQUIRE(readFacade.Affects(StmtEntity::kAssign, 10) == std::unordered_set<statementNumber>({ 1, 2, 4, 6, 8, 9 }));
    REQUIRE(readFacade.Affects(StmtEntity::kAssign, 11) == std::unordered_set<statementNumber>({ 10 }));
    REQUIRE(readFacade.Affects(StmtEntity::kAssign, 12) == std::unordered_set<statementNumber>({ 1, 4, 8, 10, 11 }));
    REQUIRE(readFacade.Affects(StmtEntity::kAssign, 13) == std::unordered_set<statementNumber>({ }));
    REQUIRE(readFacade.Affects(StmtEntity::kAssign, 14) == std::unordered_set<statementNumber>({ 13 }));
    REQUIRE(readFacade.Affects(StmtEntity::kAssign, 51) == std::unordered_set<statementNumber>({ }));


    REQUIRE(readFacade.Affects(StmtEntity::kCall, 14) == std::unordered_set<statementNumber>({}));
    REQUIRE(readFacade.Affects(1, StmtEntity::kCall) == std::unordered_set<statementNumber>({}));

    std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash> affects = readFacade.Affects();

    std::unordered_set<std::pair<statementNumber, statementNumber>, PairHash> affectsRequired = { {4, 12}, {1, 12}, {4, 4}, {1, 4}, {9, 10}, {1, 8}, {1, 10}, {10, 11}, {2, 6}, {10, 12}, {8, 10}, {2, 10}, {6, 6}, {4, 8}, {4, 10}, {6, 10}, {8, 12}, {11, 12}, {13, 14}};

    REQUIRE(affects == affectsRequired);
}

TEST_CASE("Test affects testcase 2") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    write_facade writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    QPS qps(readFacade);
    std::string simpleProgram = R"(procedure Third {
          z = 5;
          v = a;
          print v; })";

    sourceProcessor.processSource(simpleProgram);
    REQUIRE(!readFacade.isAffects(1, 3));
    REQUIRE(!readFacade.isAffects(Wildcard(), Wildcard()));
    REQUIRE(!readFacade.isAffects(1, Wildcard()));
    REQUIRE(!readFacade.isAffects(Wildcard(), 3));
    REQUIRE(!readFacade.isAffects(1, 2));
    REQUIRE(!readFacade.isAffects(2, 3));
    REQUIRE(!readFacade.isAffects(3, 3));
    REQUIRE(!readFacade.isAffects(2, 1));
}

TEST_CASE("Test failing modifies testcase") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    write_facade writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    QPS qps(readFacade);
    std::string simpleProgram = R"(procedure q {
                                      call reader;
                                      if (a==1+3) then {
                                        a = a + 1; }
                                      else {
                                        z = p + x; } }

                                    procedure reader {
                                      read c;
                                      print d; }

                                    procedure hobbs {
                                      print a;
                                      print b; })";

    std::string query_1 = "variable v; Select v such that Modifies(\"printResults\", _)";
    sourceProcessor.processSource(simpleProgram);
    std::unordered_set<std::string> temp = qps.Evaluate(query_1);
    Wildcard w = Wildcard();
    std::unordered_set<std::string> modifies = readFacade.modifiesProcedure("q");

    REQUIRE(qps.Evaluate(query_1) == std::unordered_set<std::string>({}));
}

TEST_CASE("Test call failing testcase") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    write_facade writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    QPS qps(readFacade);
    std::string simpleProgram = R"(
        procedure one {
            call two;
        }

        procedure two {
            x = 0;
            i = 5;
            while (i!=0) {
                a = a + b;
                call three;
                c = b - a; }
            if (a==1) then {
                call six;
                t = x+1; }
            else {
                z = 1; }
            a = a + b + c;
            a = a + 2;
            a = a * b + z;
        }

        procedure three {
            if (v == z) then {
                call five;
            } else {
                call four;
            }
            z = 5;
            print v;
        }

        procedure four {
            read g;
        }

        procedure five {
            read l;
            call six;
        }

        procedure six {
            print g;
        }

        procedure seven {
            print g;
            call two;
        }
     )";
    std::string query_1 = "procedure p;Select p such that Calls(_,_)";
    sourceProcessor.processSource(simpleProgram);
    bool result = qps.Evaluate(query_1) == std::unordered_set<std::string>({ "five", "one", "seven", "three", "two", "four", "six" });

    std::unordered_set<std::string> temp = qps.Evaluate(query_1);
    REQUIRE(qps.Evaluate(query_1) == std::unordered_set<std::string>({ "five", "one", "seven", "three", "two", "four", "six" }));
}
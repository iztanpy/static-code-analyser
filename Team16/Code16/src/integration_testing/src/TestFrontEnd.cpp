#include <memory>
#include <unordered_set>
#include <string>
#include "SP/TNode.h"
#include "catch.hpp"
#include "SP/SourceProcessor.h"
#include "PKB/API/WriteFacade.h"
#include "PKB/API/ReadFacade.h"
#include "qps/qps.h"

TEST_CASE("One print statement 1") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    WriteFacade writeFacade = WriteFacade(*pkb_ptr);
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
    WriteFacade writeFacade = WriteFacade(*pkb_ptr);
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
    WriteFacade writeFacade = WriteFacade(*pkb_ptr);
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
    WriteFacade writeFacade = WriteFacade(*pkb_ptr);

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
    WriteFacade writeFacade = WriteFacade(*pkb_ptr);

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
    WriteFacade writeFacade = WriteFacade(*pkb_ptr);

    REQUIRE_THROWS(writeFacade.storeCalls({ {"main", {"p"}}, {"p", {"main"}}}));
    REQUIRE_THROWS(writeFacade.storeCalls({ {"main", {"p"}}, {"p", {"q"}}, {"q", {"main"}}}));
}

TEST_CASE("Test Calls ") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();

    ReadFacade readFacade = ReadFacade(*pkb_ptr);
    WriteFacade writeFacade = WriteFacade(*pkb_ptr);
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
    WriteFacade writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);

    std::string simpleProgram = "procedure p {x = z - 3 + I; x = x + 1; y = y + z + 4;}";

    sourceProcessor.processSource(simpleProgram);

    REQUIRE((readFacade.getAllAssigns() == std::unordered_set<statementNumber>({ 1, 2, 3 })));

    REQUIRE((readFacade.getAssignPair(PartialExpr{ "x" })
        == std::unordered_set<std::pair<statementNumber, variable>, PairHash>({ {2, "x"} })));

    REQUIRE((readFacade.getAssignPair(PartialExpr{ "y" })
        == std::unordered_set<std::pair<statementNumber, variable>, PairHash>({ {3, "y"} })));

    REQUIRE((readFacade.getAssignPair(PartialExpr{ "z" })
        == std::unordered_set<std::pair<statementNumber, variable>, PairHash>({ {1, "x"}, {3, "y"} })));

    Wildcard wildcard = Wildcard();

    REQUIRE((readFacade.getAssignPair(wildcard)
        == std::unordered_set<std::pair<statementNumber, variable>, PairHash>({ {1, "x"}, {2, "x"}, {3, "y"} })));

    REQUIRE((readFacade.getAssigns(wildcard, PartialExpr{ "x" }) == std::unordered_set<statementNumber>({ 2 })));

    REQUIRE((readFacade.getAssigns(wildcard, PartialExpr{ "y" }) == std::unordered_set<statementNumber>({ 3 })));

    REQUIRE((readFacade.getAssigns(wildcard, PartialExpr{ "z" }) == std::unordered_set<statementNumber>({ 1, 3 })));

    REQUIRE((readFacade.getAssigns(wildcard, wildcard) == std::unordered_set<statementNumber>({ 1, 2, 3 })));

    REQUIRE((readFacade.getAssigns("x", PartialExpr{ "z" }) == std::unordered_set<statementNumber>({ 1 })));

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
    WriteFacade writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    QPS qps(readFacade);

    // Initialize SP and SP tokenizer
    std::string simpleProgram = "procedure p {x = x + 1;}";
    std::string query_1 = "variable v; Select v";
    std::string query_2 = "constant c; Select c";

    sourceProcessor.processSource(simpleProgram);

    std::unordered_set<std::string> varSet = std::unordered_set<std::string>({ "x" });
    std::unordered_map<std::string, std::unordered_set<std::string>>
        varUseMap = std::unordered_map<std::string, std::unordered_set<std::string>>(
            { {"x", varSet} });
    std::unordered_set<std::string> constSet = std::unordered_set<std::string>({ "1" });
    std::unordered_map<std::string, std::unordered_set<std::string>>
        constUseMap = std::unordered_map<std::string, std::unordered_set<std::string>>(
            { {"x", constSet} });
    std::unordered_map<int, std::unordered_set<std::string>>
        assignLinePartialRHSPatternMap = std::unordered_map<int, std::unordered_set<std::string>>({ {1, {"x", "1", "x+1"}} });
    std::unordered_map<int, std::string> usesLineLHSMap = std::unordered_map<int, std::string>({ {1, "x"} });
    std::unordered_map<int, std::unordered_set<std::string>>
        usesLineRHSVarMap = std::unordered_map<int, std::unordered_set<std::string>>({ {1, {"x"}} });

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
    WriteFacade writeFacade = WriteFacade(*pkb_ptr);
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
    WriteFacade writeFacade = WriteFacade(*pkb_ptr);
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
    WriteFacade writeFacade = WriteFacade(*pkb_ptr);
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
    WriteFacade writeFacade = WriteFacade(*pkb_ptr);
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
    WriteFacade writeFacade = WriteFacade(*pkb_ptr);
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
    WriteFacade writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    QPS qps(readFacade);

    std::string
        simpleProgram = "procedure c {x = z - 3 + I - \n 100 + \t u100 + U48ka - \n \t OoOhd;} procedure procedure "
        "{ \t  read r; y = y + 4;}";
    std::string query_1 = "assign a; Select a";

    sourceProcessor.processSource(simpleProgram);
    REQUIRE(qps.Evaluate(query_1) == std::unordered_set<std::string>({ "1", "3" }));
}
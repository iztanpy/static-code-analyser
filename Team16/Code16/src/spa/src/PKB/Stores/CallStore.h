#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <utility>
#include "utils/hash_utils.h"
#include "utils/entity_types.h"
#include "qps/declaration.h"

class CallStore {
 private:
  typedef std::string procedure;
  typedef int statementNumber;
  std::unordered_map<procedure, std::unordered_set<procedure>> callTable;
  std::unordered_map<procedure, std::unordered_set<procedure>> callTableReverse;
  std::unordered_map<procedure, std::unordered_set<procedure>> callTableStar;
  std::unordered_map<procedure, std::unordered_set<procedure>> callTableStarReverse;
  std::unordered_set<std::pair<statementNumber, procedure>, PairHash> callPairs;

 public:
  CallStore();

  /**
  * @brief Stores a call relationship table.
  *
  * This function stores a call relationship table, mapping procedures to the set of procedures
  * they call.
  *
  * @param callTable An unordered_map where the key is a procedure, and the value is a set of
  *                  procedures called by the key procedure.
  */
  void storeCalls(std::unordered_map<procedure, std::unordered_set<procedure>> callTable);

  void storeCallPairs(std::unordered_map<statementNumber, procedure> callStatements);

  std::unordered_set<std::pair<statementNumber, procedure>, PairHash> getCallPairs();

  /**
  * @brief Retrieves the Call* (transitive call) relationship table.
  *
  * This function returns a map representing the transitive call relationship among procedures.
  *
  * @return An unordered_map where the key is a procedure, and the value is a set of procedures
  *         called directly or indirectly by the key procedure.
  */
  std::unordered_map<procedure, std::unordered_set<procedure>> getCallStar();

  /**
  * @brief Retrieves the set of procedures that directly call another procedure.
  *
  * This function returns a set of procedures that directly call another specified procedure.
  *
  * @param procedure1 The calling procedure.
  * @param procedure2 The called procedure.
  * @return A set of procedures calling the specified procedure.
  */
  std::unordered_set<std::pair<procedure, procedure>, PairHash> call(StmtEntity procedure1, StmtEntity procedure2);

  /**
  * @brief Retrieves the set of procedures that call a specified procedure matching a wildcard.
  *
  * This function returns a set of procedures that call a procedure matching the provided wildcard.
  *
  * @param procedure The procedure to match against wildcard.
  * @param wildcard The wildcard used to match against calling procedures.
  * @return A set of procedures calling procedures matching the wildcard.
  */
  std::unordered_set<procedure> call(StmtEntity procedure, Wildcard wildcard);

  /**
  * @brief Retrieves the set of procedures that match a wildcard and are called by a specified procedure.
  *
  * This function returns a set of procedures that match a wildcard and are called by the provided procedure.
  *
  * @param wildcard The wildcard used to match against called procedures.
  * @param procedure The calling procedure.
  * @return A set of procedures called by the procedure and matching the wildcard.
  */
  std::unordered_set<procedure> call(Wildcard wildcard, StmtEntity procedure);

  /**
  * @brief Retrieves the set of procedures called by a specified procedure.
  *
  * This function returns a set of procedures directly called by the provided procedure.
  *
  * @param p The calling procedure.
  * @return A set of procedures called by the provided procedure.
  */
  std::unordered_set<procedure> getCallChildren(procedure p);

  /**
  * @brief Retrieves the set of procedures that directly call a specified procedure.
  *
  * This function returns a set of procedures that directly call the provided procedure.
  *
  * @param p The called procedure.
  * @return A set of procedures that directly call the provided procedure.
  */
  std::unordered_set<procedure> getCallParents(procedure p);

  /**
  * @brief Checks if there is a Call relationship between two procedures matching wildcards.
  *
  * This function checks if there is a Call relationship between two procedures that match
  * the provided wildcards.
  *
  * @param wildcard The wildcard for the calling procedure.
  * @param wildcard2 The wildcard for the called procedure.
  * @return True if a Call relationship exists; false otherwise.
  */
  bool isCall(Wildcard wildcard, Wildcard wildcard2);

  /**
  * @brief Checks if a specific procedure calls any procedures matching a wildcard.
  *
  * This function checks if the specified procedure calls any procedures that match the
  * provided wildcard.
  *
  * @param wildcard The wildcard for the called procedure.
  * @param p The calling procedure.
  * @return True if the calling procedure calls any procedures matching the wildcard; false otherwise.
  */
  bool isCall(Wildcard wildcard, procedure p);

  /**
  * @brief Checks if a specific procedure is called by any procedures matching a wildcard.
  *
  * This function checks if the specified procedure is called by any procedures that match
  * the provided wildcard.
  *
  * @param p The called procedure.
  * @param wildcard The wildcard for the calling procedures.
  * @return True if the called procedure is called by any procedures matching the wildcard; false otherwise.
  */
  bool isCall(procedure p, Wildcard wildcard);

  /**
  * @brief Checks if a specific procedure calls another specific procedure.
  *
  * This function checks if a specified procedure calls another specified procedure.
  *
  * @param p The calling procedure.
  * @param p2 The called procedure.
  * @return True if the calling procedure calls the called procedure; false otherwise.
  */
  bool isCall(procedure p, procedure p2);

  /**
  * @brief Retrieves the Call* (transitive call) relationship between two procedures.
  *
  * This function returns all pairs of procedures representing the transitive call
  * relationship between two specified procedures.
  *
  * @param procedure1 The calling procedure.
  * @param procedure2 The called procedure.
  * @return A set of pairs, where each pair represents a procedure that directly or indirectly
  *         calls another procedure (procedure1, procedure2).
  */
  std::unordered_set<std::pair<procedure, procedure>, PairHash>
  callStar(StmtEntity procedure1, StmtEntity procedure2);

  /**
  * @brief Retrieves procedures that have a Call* (transitive call) relationship with a procedure matching a wildcard.
  *
  * This function returns procedures that have a transitive call relationship with a procedure
  * that matches the provided wildcard.
  *
  * @param procedure The procedure to match against wildcard.
  * @param wildcard The wildcard used to match against procedures with transitive call relationships.
  * @return A set of procedures that have a Call* relationship with a procedure matching the wildcard.
  */
  std::unordered_set<procedure> callStar(StmtEntity procedure, Wildcard wildcard);

  /**
  * @brief Retrieves procedures matching a wildcard that have a Call* (transitive call) relationship with a specific procedure.
  *
  * This function returns procedures matching a wildcard that have a transitive call relationship with
  * the provided procedure.
  *
  * @param wildcard The wildcard used to match against procedures with transitive call relationships.
  * @param procedure The procedure with which procedures are checked for Call* relationships.
  * @return A set of procedures matching the wildcard that have a Call* relationship with the provided procedure.
  */
  std::unordered_set<procedure> callStar(Wildcard wildcard, StmtEntity procedure);

  /**
  * @brief Retrieves procedures that are directly or indirectly called by a specified procedure.
  *
  * This function returns procedures that are directly or indirectly called by the provided procedure.
  *
  * @param p The calling procedure.
  * @return A set of procedures that are directly or indirectly called by the provided procedure.
  */
  std::unordered_set<procedure> getCallStarChildren(procedure p);

  /**
  * @brief Retrieves procedures that directly or indirectly call a specified procedure.
  *
  * This function returns procedures that directly or indirectly call the provided procedure.
  *
  * @param p The called procedure.
  * @return A set of procedures that directly or indirectly call the provided procedure.
  */
  std::unordered_set<procedure> getCallStarParents(procedure p);

  /**
  * @brief Checks if there is a Call* (transitive call) relationship between two procedures matching wildcards.
  *
  * This function checks if there is a transitive Call* relationship between two procedures that match
  * the provided wildcards.
  *
  * @param wildcard The wildcard for the calling procedure.
  * @param wildcard2 The wildcard for the called procedure.
  * @return True if a Call* (transitive call) relationship exists; false otherwise.
  */
  bool isCallStar(Wildcard wildcard, Wildcard wildcard2);

  /**
  * @brief Checks if a specific procedure has a Call* (transitive call) relationship with any procedures matching a wildcard.
  *
  * This function checks if the specified procedure has a transitive Call* relationship with any procedures
  * that match the provided wildcard.
  *
  * @param wildcard The wildcard for the called procedure.
  * @param p The calling procedure.
  * @return True if the calling procedure has a transitive Call* relationship with procedures matching
  *         the wildcard; false otherwise.
  */
  bool isCallStar(Wildcard wildcard, procedure p);

  /**
  * @brief Checks if a specific procedure is directly or indirectly called by any procedures matching a wildcard.
  *
  * This function checks if the specified procedure is directly or indirectly called by procedures that match
  * the provided wildcard.
  *
  * @param p The called procedure.
  * @param wildcard The wildcard for the calling procedures.
  * @return True if the called procedure is directly or indirectly called by procedures matching the wildcard;
  *         false otherwise.
  */
  bool isCallStar(procedure p, Wildcard wildcard);

  /**
  * @brief Checks if a specific procedure has a Call* (transitive call) relationship with another specific procedure.
  *
  * This function checks if a specified procedure has a transitive Call* relationship with another specified procedure.
  *
  * @param p The calling procedure.
  * @param p2 The called procedure.
  * @return True if the calling procedure has a transitive Call* relationship with the called procedure;
  *         false otherwise.
  */
  bool isCallStar(procedure p, procedure p2);
};

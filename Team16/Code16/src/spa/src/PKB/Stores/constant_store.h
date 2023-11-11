#pragma once
#include <string>
#include <unordered_set>

class constant_store {
 private:
  typedef std::string constant;
  std::unordered_set<constant> constants;

 public:
  constant_store();

  /**
  * @brief Adds a set of constants to the store.
  *
  * This method adds the given set of constants to the store, allowing them to be accessed and manipulated.
  *
  * @param constants An unordered set of constants to add to the store.
  */
  void addConstants(std::unordered_set<constant> constants);

  /**
  * @brief Retrieves all constants stored in the store.
  *
  * This method returns an unordered set containing all the constants that have been added to the store.
  *
  * @return An unordered set of constants stored in the store.
  */
  std::unordered_set<constant> getConstants();
};

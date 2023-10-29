#pragma once

#include <string>
#include <set>
#include <cassert>

enum class RelRefType {
  FOLLOWS,
  FOLLOWST,
  PARENT,
  PARENTT,
  USESS,
  USESP,
  MODIFIESS,
  MODIFIESP,
  CALLS,
  CALLST,
  NEXT,
  NEXTT,
  AFFECTS,
  SELECT,
  ASSIGN,
  WHILE,
  IF,
  WITH
};

class RelRef {
 public:
  /*!
   * Converts a relationship reference to string representation
   * @param relRef the relationship reference
   * @return a string representation of the relationship reference
   */
  static std::string toString(RelRefType relRef);

  /*!
   * Converts a string representation to a relationship reference
   * @param relRef_str the string representation
   * @return a relationship reference from string representation
   */
  static RelRefType fromString(const std::string& relRef_str);

  /*!
   * Returns a set of string representations of the relationship references
   * @return the set of string representations of the relationship references
   */
  static std::set<std::string> getStringRelRef();

  /*!
   * Return score of clause. The lower the score, the higher the priority
   * @param rel_ref
   * @param num_synonym
   * @return
   */
  static int getClauseScore(RelRefType rel_ref, int num_synonym);
};

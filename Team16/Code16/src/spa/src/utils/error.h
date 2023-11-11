#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

// Created by Shaine
// ai-gen start (gpt3, 2)
/**
 * @class error
 * @brief A base class for custom exception handling.
 */
class error : public std::exception {
 public:
  /**
  * @brief Constructs an error object with an optional error message.
  * @param msg The error message (default is "An error occurred").
  */
  explicit error(const std::string& msg = "An error occurred") : message(msg) {}

  /**
   * @brief Returns a C-style string describing the exception.
   * @return A pointer to the error message.
   */
  const char* what() const noexcept override {
    return message.c_str();
  }

  /**
   * @brief Logs the error message to the standard error stream (std::cerr).
   */
  virtual void log() const {
    std::cerr << "error: " << message << std::endl;
  }

  /**
   * @brief Logs a custom error message to the standard error stream (std::cerr).
   * @param customMessage The custom error message to log.
   */
  virtual void log(const std::string& customMessage) {
    std::cerr << "error: " << customMessage << std::endl;
  }

 protected:
  std::string message;
};

class InvalidStatementError : public error {
 public:
  /**
  * @brief Constructs an InvalidStatementError object with an optional error message.
  * @param msg The error message (default is "Invalid statement").
  */
  explicit InvalidStatementError(const std::string& msg = "Invalid statement :(") : error(msg) {}
  /**
   * @brief Logs the error message specific to invalid statements.
   * Overrides the base class log() function.
   */
  void log() const override;
};

class InvalidTokenTypeError : public error {
 public:
  /**
  * @brief Constructs an InvalidTokenTypetError object with an optional error message.
  * @param msg The error message (default is "Invalid TokenType").
  */
  explicit InvalidTokenTypeError(const std::string& msg = "Invalid Token Type :(") : error(msg) {}
  /**
   * @brief Logs the error message specific to invalid statements.
   * Overrides the base class log() function.
   */
  void log() const override;
};

class InvalidSyntaxError : public error {
 public:
  /**
  * @brief Constructs an InvalidSyntaxError object with an optional error message.
  * @param msg The error message (default is "Invalid SIMPLE Syntax.").
  */
  explicit InvalidSyntaxError(const std::string& msg = "Invalid SIMPLE Syntax :(.") : error(msg) {}
  /**
   * @brief Logs the error message specific to invalid statements.
   * Overrides the base class log() function.
   */
  void log() const override;
};

class InvalidSemanticError : public error {
 public:
  /**
  * @brief Constructs an InvalidSemanticError object with an optional error message.
  * @param msg The error message (default is "Invalid Semantic error.").
  */
  explicit InvalidSemanticError(const std::string& msg = "Invalid Semantic error. ") : error(msg) {}
  /**
   * @brief Logs the error message specific to invalid statements with semantic errors.
   * Overrides the base class log() function.
   */
  void log() const override;
};






#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

// Created by Shaine
// ai-gen start (gpt3, 2)
/**
 * @class Error
 * @brief A base class for custom exception handling.
 */
class Error : public std::exception {
 public:
     /**
     * @brief Constructs an Error object with an optional error message.
     * @param msg The error message (default is "An error occurred").
     */
    explicit Error(const std::string& msg = "An error occurred") : message(msg) {}

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
    virtual void log() {
       std::cerr << "Error: " << message << std::endl;
    }

    /**
     * @brief Logs a custom error message to the standard error stream (std::cerr).
     * @param customMessage The custom error message to log.
     */
    virtual void log(const std::string& customMessage) {
        std::cerr << "Error: " << customMessage << std::endl;
    }

 protected:
    std::string message;
};



class InvalidStatementError : public Error {
 public:
     /**
     * @brief Constructs an InvalidStatementError object with an optional error message.
     * @param msg The error message (default is "Invalid statement").
     */
    explicit InvalidStatementError(const std::string& msg = "Invalid statement") : Error(msg) {}
    /**
     * @brief Logs the error message specific to invalid statements.
     * Overrides the base class log() function.
     */
    void log() override;
};


class InvalidTokenTypeError : public Error {
 public:
    /**
    * @brief Constructs an InvalidSTokenTypetError object with an optional error message.
    * @param msg The error message (default is "Invalid TokenType").
    */
    explicit InvalidTokenTypeError(const std::string& msg = "Invalid Token Type") : Error(msg) {}
    /**
     * @brief Logs the error message specific to invalid statements.
     * Overrides the base class log() function.
     */
    void log() override;
};


class InvalidSyntaxError : public Error {
 public:
    /**
    * @brief Constructs an InvalidSyntaxError object with an optional error message.
    * @param msg The error message (default is "Invalid SIMPLE Syntax.").
    */
    explicit InvalidSyntaxError(const std::string& msg = "Invalid SIMPLE Syntax. Unable to build AST.") : Error(msg) {}
    /**
     * @brief Logs the error message specific to invalid statements.
     * Overrides the base class log() function.
     */
    void log() override;
};


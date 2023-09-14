#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

class Error : public std::exception {
 public:
    explicit Error(const std::string& msg = "An error occurred") : message(msg) {}
    const char* what() const noexcept override {
       return message.c_str();
    }
    virtual void log() {
       std::cerr << "Error: " << message << std::endl;
    }

    virtual void log(const std::string& customMessage) {
        std::cerr << "Error: " << customMessage << std::endl;
    }
 protected:
    std::string message;
};



class InvalidStatementError : public Error {
 public:
    explicit InvalidStatementError(const std::string& msg = "Invalid statement") : Error(msg) {}
    void log() override;
};

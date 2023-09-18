#pragma once

#include <vector>

class DesignExtractor;  // Forward declaration
/**
 * @class Parser
 * @brief Abstract base class for parsing operations on a sequence of tokens.
 *
 * The `Parser` class defines the common interface for parsing operations on a sequence of tokens.
 * Subclasses of `Parser` are expected to provide specific implementations for parsing different parts of the input.
 *
 * @note This class is designed to be abstract and cannot be instantiated directly. Subclasses must override the
 *       `parse` method to perform custom parsing logic.
 **/
class Parser {
 public:
    Parser() = default;
    virtual ~Parser() = default;
    virtual int parse(const std::vector<Token>& tokens, int curr_index) = 0;
    DesignExtractor* designExtractor = new DesignExtractor();  // Initialize to nullptr in the constructor
};

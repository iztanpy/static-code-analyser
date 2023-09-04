#include "SPTokeniser.h"

std::vector<std::string> SPtokeniser::tokenise(const std::string& input) {
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    std::string token;

    while (std::getline(iss, token, ' ')) {
        tokens.push_back(token);
    }

    return tokens;
}

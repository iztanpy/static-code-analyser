#include <string>
#include "qps/query_parser.h"
#include "qps/clause_director.h"

ParsedQuery QueryParser::ParseTokenizedQuery(const std::vector<QueryToken>& tokens) {
    /* TODO: validates the tokens */

    // from the tokens, identify the declarations together with its synonyms
    std::vector<Declaration> declarations = ExtractDeclarations(tokens);
    // from the tokens, look for select/ such that/ pattern clauses and call the builders
    SelectClause selectClause;
    for (const QueryToken& token : tokens) {
        switch (token.type) {
            case TokenType::SELECT:
                selectClause = ExtractSelectClause(token, declarations);
            default:
                /* TODO: extract such that and pattern clauses */
                continue;
        }
    }
    ParsedQuery parsedQuery = {selectClause};
    return parsedQuery;
}

std::vector<Declaration> QueryParser::ExtractDeclarations(const std::vector<QueryToken>& tokens) {
    TokenType previous_token_type;
    DesignEntity current_entity_type;
    std::vector<Declaration> declarations;
    for (const QueryToken& token : tokens) {
        switch (token.type) {
            case TokenType::DECLARATION:
                current_entity_type = fromString(token.text);
                previous_token_type = token.type;
            case TokenType::SYNONYM:
                if (previous_token_type == TokenType::DECLARATION) {
                    std::string synonym = token.text;
                    Declaration new_declaration = {synonym, current_entity_type};
                    declarations.push_back(new_declaration);
                }
            default:
                previous_token_type = token.type;
        }
    }
    return declarations;
}

SelectClause QueryParser::ExtractSelectClause(const QueryToken& token, const std::vector<Declaration>& declarations) {
    // invoke builder design pattern
    SelectClauseBuilder builder;
    ClauseDirector::makeSelectClause(builder, token, declarations);
    return builder.getClause();
}

#include <string>
#include "qps/query_parser.h"
#include "qps/clause_director.h"

ParsedQuery QueryParser::ParseTokenizedQuery(const std::vector<QueryToken>& tokens) {
    /* TODO: validates the tokens */
    ParsedQuery parsedQuery;
    // from the tokens, identify the declarations together with its synonyms
    std::vector<Declaration> declarations = ExtractDeclarations(tokens);
    // from the tokens, look for select/ such that/ pattern clauses and call the builders
    SelectClause selectClause;
    std::vector<QueryToken> selectTokens = ExtractSelectTokens(tokens);
    std::vector<SelectClause> selectClauses = ExtractSelectClauses(selectTokens, declarations);
    /* For now, we only have one synonym for select clause
     * hence we can just return the first value of the vector
     * - open to extensions in the future
     */
    if (selectClauses.size() == 1) {
        parsedQuery.select = selectClauses[0];
    }
    /* TODO: extract such that and pattern clauses */
    return parsedQuery;
}

std::vector<Declaration> QueryParser::ExtractDeclarations(const std::vector<QueryToken>& tokens) {
    TokenType previous_token_type;
    DesignEntity current_entity_type;
    std::vector<Declaration> declarations;
    for (const QueryToken& token : tokens) {
        switch (token.type) {
            case TokenType::DECLARATION:
                current_entity_type = Entity::fromString(token.text);
                previous_token_type = token.type;
                break;
            case TokenType::SYNONYM:
                if (previous_token_type == TokenType::DECLARATION) {
                    std::string synonym = token.text;
                    Declaration new_declaration = {synonym, current_entity_type};
                    declarations.push_back(new_declaration);
                }
                break;
            default:
                previous_token_type = token.type;
        }
    }
    return declarations;
}

std::vector<QueryToken> QueryParser::ExtractSelectTokens(const std::vector<QueryToken>& tokens) {
    TokenType previous_token_type;
    std::vector<QueryToken> selectTokens;
    for (const QueryToken& token : tokens) {
        switch (token.type) {
            case TokenType::SELECT:
                previous_token_type = token.type;
                break;
            case TokenType::SYNONYM:
                if (previous_token_type == TokenType::SELECT) {
                    selectTokens.push_back(token);
                }
                break;
            default:
                previous_token_type = token.type;
        }
    }
    return selectTokens;
}

std::vector<SelectClause> QueryParser::ExtractSelectClauses(const std::vector<QueryToken> &selectTokens,
                                                            const std::vector<Declaration> &declarations) {
    std::vector<SelectClause> selectClauses;
    // invoke builder design pattern
    for (const QueryToken& token : selectTokens) {
        SelectClauseBuilder builder;
        selectClauses.push_back(ClauseDirector::makeSelectClause(builder, token, declarations));
    }
    return selectClauses;
}

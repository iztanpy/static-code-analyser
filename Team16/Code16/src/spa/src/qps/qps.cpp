#include "qps/qps.h"

QPS::QPS(ReadFacade& pkb_reader) : query_evaluator(pkb_reader) {}

std::unordered_set<std::string> QPS::Evaluate(std::string& query) {
  try {
    ParsedQuery parsed_query = QueryParser::ParseTokenizedQuery(query);
    std::unordered_set<std::string> results = query_evaluator.Evaluate(parsed_query);
    if (results.empty()) {
      return results;
    }
    return results;
  } catch (const QpsSyntaxError& e) {
//    return {"SyntaxError"};
    return {"Hello"};
  } catch (const QpsSemanticError& e) {
//    return {"SemanticError"};
    return {"World"};
  } catch (...) {
    return {"SemanticError"};
  }
}

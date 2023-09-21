#include "qps/qps.h"

QPS::QPS(ReadFacade& pkb_reader) : query_evaluator(pkb_reader) {}

std::unordered_set<std::string> QPS::Evaluate(std::string query) {
  std::vector<QueryToken> query_tokens = QueryTokenizer::tokenize(query);

  ParsedQuery parsed_query = QueryParser::ParseTokenizedQuery(query_tokens);

  // TODO(phuccuongngo99): Try catch here please
  return query_evaluator.Evaluate(parsed_query);
}

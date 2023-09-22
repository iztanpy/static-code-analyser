#include "qps/qps.h"

QPS::QPS(ReadFacade & pkb_reader) : query_evaluator(pkb_reader) {}

std::unordered_set<std::string> QPS::Evaluate(std::string query) {
  ParsedQuery parsed_query = QueryParser::ParseTokenizedQuery(query);

  // TODO(phuccuongngo99): Try catch here please
  return query_evaluator.Evaluate(parsed_query);
}

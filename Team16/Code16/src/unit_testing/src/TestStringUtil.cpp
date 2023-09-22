#pragma once

#include <catch.hpp>
#include "utils/string_utils.h"

TEST_CASE("Test splitting by delimiter") {
  std::string sample_query_1 = "variable v; Select v";
  std::vector<std::string> expected_results_1 = {"variable v", "Select v"};
  std::vector<std::string> results_1 = string_util::SplitStringBy(';', sample_query_1);
  REQUIRE(expected_results_1 == results_1);

  std::string sample_query_2 = "variable v1, v2; assign a1; Select a1";
  std::vector<std::string> expected_results_2 = {"variable v1, v2", "assign a1", "Select a1"};
  std::vector<std::string> results_2 = string_util::SplitStringBy(';', sample_query_2);
  REQUIRE(expected_results_2 == results_2);

  std::string sample_query_3 = "\t\t variable v1, v2 \n\n; \t assign a1 \t; \n Select a1 \t";
  std::vector<std::string> expected_results_3 = {"variable v1, v2", "assign a1", "Select a1"};
  std::vector<std::string> results_3 = string_util::SplitStringBy(';', sample_query_3);
  REQUIRE(expected_results_3 == results_3);
}
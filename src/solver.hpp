//
// Created by longlp on 11/12/2019.
//

#ifndef SRC_SOLVER_HPP_
#define SRC_SOLVER_HPP_
#include "defs.hpp"
namespace dss {
  struct CriteriaVector {
    double criteria_rank;
    double criteria_fee;
    double criteria_point;
    double criteria_quota;
  };
  struct SolveResult {
    std::vector<DecisionRecord> decision_records;
    CriteriaVector              s_plus_vector;
    CriteriaVector              s_minus_vector;
  };
  auto solve(const InputParams& input, const std::vector<ReceivedDatabaseRecord>& db_records)
      -> SolveResult;
}  // namespace dss

#endif  // SRC_SOLVER_HPP_

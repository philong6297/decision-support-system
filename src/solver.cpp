//
// Created by longlp on 11/12/2019.
//

#include "solver.hpp"
#include <cmath>
namespace {
  using dss::CriteriaVector;
  using dss::DecisionRecord;
  using dss::InputParams;
  using dss::ReceivedDatabaseRecord;
  // good 0 bad 1
  constexpr auto kGood = 0.0;
  constexpr auto kBad  = 1.0;

  // heuristic
  auto calculate_criteria_fee(const int32_t input_fee,
                              const int32_t school_fee,
                              const double  weight_fee) -> double {
    if (input_fee >= 2 * school_fee) {
      return kGood;
    }
    if (input_fee <= school_fee / 2) {
      return kBad;
    }
    return weight_fee * (2 * input_fee - school_fee) / (3 * school_fee);
  }
  // heuristic
  auto calculate_criteria_point(const double input_point,
                                const double school_point,
                                const double weight_point) -> double {
    const auto u_1 = std::max(school_point - 4, 0.0);
    const auto u_2 = std::min(school_point + 4, 30.0);
    if (input_point >= u_2) {
      return kGood;
    }
    if (input_point <= u_1) {
      return kBad;
    }
    return weight_point * (input_point - u_1) / (u_2 - u_1);
  }

  // school rank need to be normalized
  auto calculate_criteria_rank(const double normalized_rank, const double weight_rank) -> double {
    return normalized_rank * weight_rank;
  }

  // quota need to be normalized
  auto calculate_criteria_quota(const double normalized_quota, const double weight_quota)
      -> double {
    return normalized_quota * weight_quota;
  }

  auto calculate_distance(const CriteriaVector& vector_a, const CriteriaVector& vector_b)
      -> double {
    const auto sqr_fee = (vector_a.criteria_fee - vector_b.criteria_fee) *
                         (vector_a.criteria_fee - vector_b.criteria_fee);
    const auto sqr_point = (vector_a.criteria_point - vector_b.criteria_point) *
                           (vector_a.criteria_point - vector_b.criteria_point);
    const auto sqr_rank = (vector_a.criteria_rank - vector_b.criteria_rank) *
                          (vector_a.criteria_rank - vector_b.criteria_rank);
    const auto sqr_quota = (vector_a.criteria_quota - vector_b.criteria_quota) *
                           (vector_a.criteria_quota - vector_b.criteria_quota);
    return std::sqrt(sqr_fee + sqr_point + sqr_rank + sqr_quota);
  }

  auto calculate_performance(const double ideal_best, const double ideal_worst) -> double {
    return ideal_worst / (ideal_best + ideal_worst);
  }
}  // namespace

auto dss::solve(const InputParams& input, const std::vector<ReceivedDatabaseRecord>& db_records)
    -> dss::SolveResult {
  std::vector<DecisionRecord> decision_records;

  // 1. normalize and heuristic
  auto sum_sqr_rank  = 0.0;
  auto sum_sqr_quota = 0.0;
  for (const auto& db_record : db_records) {
    DecisionRecord decision_record{};
    // copy from db
    decision_record.quota           = db_record.quota;
    decision_record.fee             = db_record.fee;
    decision_record.university_rank = db_record.university_rank;
    decision_record.major_name      = db_record.major_name;
    decision_record.university_name = db_record.university_name;
    decision_record.point           = db_record.point;

    // heuristic calculate criteria for fee and point
    decision_record.criteria_fee =
        calculate_criteria_fee(input.desired_fee, db_record.fee, input.weight_fee);
    decision_record.criteria_point =
        calculate_criteria_point(input.point, db_record.point, input.weight_point);

    // setup for normalize rank and quota
    sum_sqr_rank += db_record.university_rank * db_record.university_rank;
    sum_sqr_quota += db_record.quota * db_record.quota;

    decision_records.push_back(std::move(decision_record));
  }
  // calculate criteria for rank and quota
  sum_sqr_rank  = std::sqrt(sum_sqr_rank);
  sum_sqr_quota = std::sqrt(sum_sqr_quota);
  for (auto& decision_record : decision_records) {
    const auto normalized_rank     = double(decision_record.university_rank) / sum_sqr_rank;
    const auto normalized_quota    = double(decision_record.quota) / sum_sqr_quota;
    decision_record.criteria_rank  = calculate_criteria_rank(normalized_rank, input.weight_rank);
    decision_record.criteria_quota = calculate_criteria_quota(normalized_quota, input.weight_quota);
  }

  // 2. update best/worst ideal
  CriteriaVector worst_ideal = {decision_records[0].criteria_rank,
                                decision_records[0].criteria_fee,
                                decision_records[0].criteria_point,
                                decision_records[0].quota};
  CriteriaVector best_ideal  = {decision_records[0].criteria_rank,
                               decision_records[0].criteria_fee,
                               decision_records[0].criteria_point,
                               decision_records[0].quota};
  for (size_t i = 0; i < std::size(decision_records); ++i) {
    // update worst ideal
    worst_ideal.criteria_point =
        std::max(worst_ideal.criteria_point, decision_records[i].criteria_point);
    worst_ideal.criteria_fee = std::max(worst_ideal.criteria_fee, decision_records[i].criteria_fee);
    worst_ideal.criteria_rank =
        std::max(worst_ideal.criteria_rank, decision_records[i].criteria_rank);
    worst_ideal.criteria_quota =
        std::max(worst_ideal.criteria_quota, decision_records[i].criteria_quota);

    // update best ideal
    best_ideal.criteria_point =
        std::min(best_ideal.criteria_point, decision_records[i].criteria_point);
    best_ideal.criteria_fee = std::min(best_ideal.criteria_fee, decision_records[i].criteria_fee);
    best_ideal.criteria_rank =
        std::min(best_ideal.criteria_rank, decision_records[i].criteria_rank);
    best_ideal.criteria_quota =
        std::min(best_ideal.criteria_quota, decision_records[i].criteria_quota);
  }

  // 3. calculate vector distance to best/worst ideal and performance score
  CriteriaVector temp_vector{};
  for (auto& decision_record : decision_records) {
    // generate vector for this record
    temp_vector.criteria_point = decision_record.criteria_point;
    temp_vector.criteria_fee   = decision_record.criteria_fee;
    temp_vector.criteria_rank  = decision_record.criteria_rank;
    temp_vector.criteria_quota = decision_record.criteria_quota;

    decision_record.s_plus_score  = calculate_distance(temp_vector, best_ideal);
    decision_record.s_minus_score = calculate_distance(temp_vector, worst_ideal);
    decision_record.performance_score =
        calculate_performance(decision_record.s_plus_score, decision_record.s_minus_score);
  }

  // 4. done
  return {decision_records, best_ideal, worst_ideal};
}

//
// Created by longlp on 04/12/2019.
//

#ifndef SRC_DEFS_HPP_
#define SRC_DEFS_HPP_
#include <QString>
#include <vector>
namespace dss {
  struct InputParams {
    QString subject;
    int32_t desired_fee;
    double  point;
    double  weight_fee;
    double  weight_point;
    double  weight_rank;
    double  weight_quota;
  };

  struct ReceivedDatabaseRecord {
    QString university_name;
    QString major_name;
    int32_t university_rank;
    int32_t fee;
    double  point;
    int32_t quota;
  };

  struct DecisionRecord {
    QString university_name;
    QString major_name;
    int32_t university_rank;
    double  criteria_rank;
    int32_t fee;
    double  criteria_fee;
    double  point;
    double  criteria_point;
    int32_t quota;
    double  criteria_quota;
    double  s_plus_score;
    double  s_minus_score;
    double  performance_score;
  };
}  // namespace dss
#endif  // SRC_DEFS_HPP_

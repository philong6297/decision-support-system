//
// Created by longlp on 10/12/2019.
//

#ifndef SRC_DATABASE_SERVICE_HPP_
#define SRC_DATABASE_SERVICE_HPP_
#include <QtSql>
#include <set>
#include <vector>
#include "defs.hpp"
class DatabaseService {
 public:
  explicit DatabaseService();
  ~DatabaseService();

  std::set<QString> FetchSubject();

  std::vector<dss::ReceivedDatabaseRecord> FetchRecordsBySubject(const QString& subject);

 private:
  QSqlDatabase database_;
};

#endif  // SRC_DATABASE_SERVICE_HPP_

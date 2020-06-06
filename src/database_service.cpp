//
// Created by longlp on 10/12/2019.
//

#include "database_service.hpp"
DatabaseService::DatabaseService() : database_(QSqlDatabase::addDatabase("QSQLITE")) {
  database_.setDatabaseName(
      "/media/longlp/share/cpp-projects/decision-support-system/data/dss2.sqlite");
  if (!database_.open()) {
    qDebug() << "Error occurred opening the database.";
    qDebug() << database_.lastError();
  }
  else {
    qDebug() << "open success";
  }
}

DatabaseService::~DatabaseService() {
  database_.close();
}

std::set<QString> DatabaseService::FetchSubject() {
  QSqlQuery         query(database_);
  std::set<QString> set;
  if (query.exec("select  distinct subject_group from major")) {
    while (query.next()) {
      // split major array
      for (const auto& subject_group : query.value(0).toString().split(QRegExp("[;./:-]"))) {
        const auto trimmed = subject_group.trimmed();
        if (trimmed.isEmpty()) {
          continue;
        }
        // filter case "A00 (...)" or Vẽ\nToán
        if (trimmed.contains(QRegExp("[()\n]"))) {
          continue;
        }
        // filter all lower case
        if (not trimmed.front().isUpper()) {
          continue;
        }
        // filter case Al
        if (trimmed.size() == 2 and not trimmed.back().isNumber()) {
          continue;
        }
        // filter case COO
        if (trimmed.size() == 3 and (not trimmed[1].isNumber() or not trimmed.back().isNumber())) {
          continue;
        }
        // filter case A 00 and A001
        if (trimmed.size() == 4 and
            (trimmed.contains(' ') or
             (trimmed[1].isNumber() and trimmed[2].isNumber() and trimmed[3].isNumber()))) {
          continue;
        }
        // filter case A00 B01
        if (trimmed.size() == 7 and trimmed[3] == ' ') {
          continue;
        }
        // filter case A00t A01
        if (trimmed.size() == 8 and (trimmed[3].isLetter() and trimmed[4] == ' ')) {
          continue;
        }

        set.insert(trimmed);
      }
    }
  }
  else {
    qDebug() << query.lastError();
  }
  return set;
}
std::vector<dss::ReceivedDatabaseRecord> DatabaseService::FetchRecordsBySubject(
    const QString& subject) {
  QSqlQuery                                query(database_);
  std::vector<dss::ReceivedDatabaseRecord> result{};
  const auto                               query_statement =
      QString(
          "select "
          "university.name, major.name, university.rank, university.tuition_fee, quota.point, "
          "quota.total "
          "from university, major, quota, "
          "(select id as major_id, university_id from major where major.subject_group like %1) a "
          "where a.major_id == major.id and "
          "a.major_id == quota.major_id and "
          "a.university_id == university.id and "
          "a.university_id == major.university_id")
          .arg("\"%" + subject + "%\"");
  if (query.exec(query_statement)) {
    while (query.next()) {
      dss::ReceivedDatabaseRecord record{};
      record.university_name = query.value(0).toString();
      record.major_name      = query.value(1).toString();
      record.university_rank = query.value(2).toInt();
      record.fee             = query.value(3).toInt();
      record.point           = query.value(4).toDouble();
      record.quota           = query.value(5).toInt();
      result.push_back(std::move(record));
    }
  }
  else {
    qDebug() << query.lastError();
  }
  return result;
}

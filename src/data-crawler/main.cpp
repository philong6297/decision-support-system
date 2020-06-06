#include <QCoreApplication>
#include <QtCore>
#include <QtSql>
#include <fstream>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
namespace {
  using nlohmann::json;
}
int main() {
  const auto reader = []() -> json {
    json result;
    std::ifstream("data/university_diemchuan.json") >> result;
    return result;
  }();

  // Create database.
  auto database = QSqlDatabase::addDatabase("QSQLITE");
  database.setDatabaseName(
      "/media/longlp/share/cpp-projects/decision-support-system/data/dss2.sqlite");
  if (!database.open()) {
    qDebug() << "Error occurred opening the database.";
    qDebug() << database.lastError();
    return -1;
  }
  QSqlQuery query(database);
  // Insert row.
  // query.prepare();
  // query.addBindValue("Some text");
  const auto execQuery = [&query](QString msg) {
    qDebug() << msg;
    if (not query.exec()) {
      qDebug() << query.lastError() << query.lastQuery();
    }
    else {
      qDebug() << "success";
    }
  };
  for (const auto& record : reader) {
    const auto& code =
        QString(record["university_meta"]["university_code"].get<std::string>().c_str()).trimmed();
    const auto& name =
        QString(record["university_meta"]["university_name"].get<std::string>().c_str()).trimmed();
    query.prepare("INSERT INTO university(code,name) VALUES (?,?);");
    query.bindValue(0, code);
    query.bindValue(1, name);
    execQuery("university");
    const auto university_id = query.lastInsertId();

    const auto& datas = record["diemchuan_datas"];
    qDebug() << datas.size();
    for (const auto& data : datas) {
      if (data["year"] == 2018) {
        const auto& major_code =
            QString::fromStdString(data["major_code"].get<std::string>()).trimmed();
        const auto& major_name =
            QString::fromStdString(data["major_name"].get<std::string>()).trimmed();
        const auto& point =
            QString::fromStdString(data["point"].get<std::string>().c_str()).trimmed().toFloat();
        const auto& subject_group =
            QString::fromStdString(data["subject_group"].get<std::string>().c_str()).trimmed();

        // major
        query.prepare("INSERT INTO major(name,code,university_id,subject_group) VALUES(?,?,?,?);");
        query.bindValue(0, major_name);
        query.bindValue(1, major_code);
        query.bindValue(2, university_id);
        query.bindValue(3, subject_group);
        execQuery("major");
        const auto major_id = query.lastInsertId();

        // point
        query.prepare("insert into standard_point(point,major_id) VALUES(?,?);");
        query.bindValue(0, point);
        query.bindValue(1, major_id);
        execQuery("point");
      }
    }
  }
  return 0;
}

//
// Created by longlp on 04/12/2019.
//

#ifndef SRC_DECISION_TABLE_WINDOW_HPP_
#define SRC_DECISION_TABLE_WINDOW_HPP_
#include <QWidget>
#include "defs.hpp"
class QTableView;
class QAbstractItemModel;
class QLabel;
class QStandardItemModel;
class DecisionTableWindow final : public QWidget {
  Q_OBJECT
 public:
  explicit DecisionTableWindow(const dss::InputParams&                         input,
                               const std::vector<dss::ReceivedDatabaseRecord>& db_records);
  ~DecisionTableWindow() override = default;
  Q_DISABLE_COPY_MOVE(DecisionTableWindow)

 signals:
  void ShowSignal() const;
  void CloseSignal() const;

 private:
  void showEvent(QShowEvent* event) override;
  void closeEvent(QCloseEvent* event) override;
  void Setup();
  void SetupUi();
  void SetupModel();
  void UpdateTable(const dss::InputParams&                         input_params,
                   const std::vector<dss::ReceivedDatabaseRecord>& db_records);

  QLabel*             user_label_;
  QLabel*             weight_label_;
  QLabel*             vector_label_;
  QTableView*         table_view_;
  QStandardItemModel* model_;
};

#endif  // SRC_DECISION_TABLE_WINDOW_HPP_

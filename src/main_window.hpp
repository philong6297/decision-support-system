//
// Created by longlp on 04/12/2019.
//

#ifndef SRC_MAIN_WINDOW_HPP_
#define SRC_MAIN_WINDOW_HPP_
#include <QWidget>
#include <QtSql>
#include <memory>
#include <vector>
#include "database_service.hpp"
#include "decision_table_window.hpp"
#include "defs.hpp"
class QComboBox;
class QSpinBox;
class QPushButton;
class QDoubleSpinBox;

class MainWindow final : public QWidget {
  Q_OBJECT
 public:
  explicit MainWindow();
  ~MainWindow() override = default;
  Q_DISABLE_COPY_MOVE(MainWindow)

 private slots:
  void OnButtonAnalyzeClicked();
  void OnButtonResetClicked();

 private:
  auto                                 Setup() -> void;
  auto                                 SetupUi() -> void;
  auto                                 SetupSignalsAndSlots() -> void;
  void                                 ResetValues();
  QComboBox*                           user_desired_subject_combo_box_;
  QDoubleSpinBox*                      user_point_spin_box_;
  QSpinBox*                            user_desired_tuition_fees_spin_box_;
  QDoubleSpinBox*                      weight_fee_spin_box_;
  QDoubleSpinBox*                      weight_point_spin_box_;
  QDoubleSpinBox*                      weight_rank_spin_box_;
  QDoubleSpinBox*                      weight_quota_spin_box_;
  QPushButton*                         reset_button_;
  QPushButton*                         analyze_button_;
  std::unique_ptr<DecisionTableWindow> decision_table_;
  std::unique_ptr<DatabaseService>     db_service_;
};

#endif  // SRC_MAIN_WINDOW_HPP_

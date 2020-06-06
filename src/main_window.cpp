//
// Created by longlp on 04/12/2019.
//

#include "main_window.hpp"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <memory>
#include <set>
#include "solver.hpp"
#include "widget_creator.hpp"
//#include "decision_table_window.hpp"

namespace {}  // namespace

MainWindow::MainWindow() :
    QWidget(nullptr),
    user_desired_subject_combo_box_(WidgetCreator::MakeComboBox(this)),
    user_point_spin_box_(WidgetCreator::MakeDoubleSpinBox(this, "", 0.0, 30.0, 1.0)),
    user_desired_tuition_fees_spin_box_(
        WidgetCreator::MakeSpinBox(this, " VND", 1'000'000, 1'000'000'000, 500'000)),
    weight_fee_spin_box_(WidgetCreator::MakeDoubleSpinBox(this, "", 0.0, 1.0, 0.02)),
    weight_point_spin_box_(WidgetCreator::MakeDoubleSpinBox(this, "", 0.0, 1.0, 0.02)),
    weight_rank_spin_box_(WidgetCreator::MakeDoubleSpinBox(this, "", 0.0, 1.0, 0.02)),
    weight_quota_spin_box_(WidgetCreator::MakeDoubleSpinBox(this, "", 0.0, 1.0, 0.02)),
    reset_button_(WidgetCreator::MakePushButton(this, "Cài Đặt Lại", false)),
    analyze_button_(WidgetCreator::MakePushButton(this, "Phân Tích", true)),
    decision_table_(nullptr),
    db_service_{std::make_unique<DatabaseService>()} {
  Setup();
}

auto MainWindow::OnButtonAnalyzeClicked() -> void {
  const auto input_params = [this]() -> dss::InputParams {
    const auto subject      = user_desired_subject_combo_box_->currentText();
    const auto desired_fee  = user_desired_tuition_fees_spin_box_->value();
    const auto point        = user_point_spin_box_->value();
    const auto weight_fee   = weight_fee_spin_box_->value();
    const auto weight_point = weight_point_spin_box_->value();
    const auto weight_rank  = weight_rank_spin_box_->value();
    const auto weight_quota = weight_quota_spin_box_->value();
    return {subject, desired_fee, point, weight_fee, weight_point, weight_rank, weight_quota};
  }();

  const auto& fetch_records = db_service_->FetchRecordsBySubject(input_params.subject);
  decision_table_           = std::make_unique<DecisionTableWindow>(input_params, fetch_records);

  decision_table_->show();
}

auto MainWindow::OnButtonResetClicked() -> void {
  ResetValues();
}

auto MainWindow::Setup() -> void {
  ResetValues();
  SetupSignalsAndSlots();
  SetupUi();
}

auto MainWindow::SetupUi() -> void {
  // main window ui
  this->setWindowTitle("HTGQĐ");
  this->setFixedSize(724 * 1.5, 633 * 1.5);

  // main window layout
  auto* main_window_layout = new QVBoxLayout(this);
  // this->setLayout(main_window_layout);
  main_window_layout->setContentsMargins(20, 20, 20, 20);
  main_window_layout->setSpacing(0);

  // main window label
  auto* main_window_label = new QLabel(this);
  QFont main_window_font("Ubuntu");
  main_window_font.setPointSize(34);
  main_window_label->setFont(main_window_font);
  main_window_label->setText("HỆ THỐNG TRỢ GIÚP LỰA CHỌN NGUYỆN VỌNG KÌ THI THPT QUỐC GIA");
  main_window_label->setScaledContents(true);
  main_window_label->setWordWrap(true);
  main_window_label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  main_window_layout->addWidget(main_window_label, 2);

  // Settings frame
  auto* settings_frame = WidgetCreator::MakeFrame(this);
  main_window_layout->addWidget(settings_frame, 4);
  auto* settings_frame_layout = new QVBoxLayout(settings_frame);
  // settings_frame->setLayout(settings_frame_layout);
  settings_frame_layout->setContentsMargins(175 * 2, 10 * 2, 175 * 2, 10 * 2);
  settings_frame_layout->setSpacing(6 * 2);

  // general setting
  auto* general_settings_group_box = WidgetCreator::MakeGroupBox(this, "Thông Tin Cơ Bản");
  settings_frame_layout->addWidget(general_settings_group_box, 2);
  auto* general_settings_layout = new QFormLayout(general_settings_group_box);
  // general_settings_group_box->setLayout(general_settings_layout);
  general_settings_layout->setContentsMargins(9 * 2, 9 * 2, 9 * 2, 9 * 2);
  general_settings_layout->setHorizontalSpacing(30 * 2);
  general_settings_layout->setVerticalSpacing(10 * 2);

  // items in general
  general_settings_layout->addRow("Ngành Nguyện Vọng", user_desired_subject_combo_box_);
  general_settings_layout->addRow("Học Phí Mong Muốn", user_desired_tuition_fees_spin_box_);
  general_settings_layout->addRow("Điểm Thi", user_point_spin_box_);

  // advance settings
  auto* advance_settings_group_box = WidgetCreator::MakeGroupBox(this, "Cài Đặt Nâng Cao");
  settings_frame_layout->addWidget(advance_settings_group_box, 3);
  auto* advance_settings_layout = new QFormLayout(advance_settings_group_box);
  // advance_settings_group_box->setLayout(general_settings_layout);
  advance_settings_layout->setContentsMargins(9 * 2, 9 * 2, 9 * 2, 9 * 2);
  advance_settings_layout->setHorizontalSpacing(30 * 2);
  advance_settings_layout->setVerticalSpacing(10 * 2);

  // items in advance
  advance_settings_layout->addRow("Trọng Số Học Phí", weight_fee_spin_box_);
  advance_settings_layout->addRow("Trọng Số Điểm", weight_point_spin_box_);
  advance_settings_layout->addRow("Trọng Số Đánh Giá Trường", weight_rank_spin_box_);
  advance_settings_layout->addRow("Trọng Số Chỉ Tiêu", weight_quota_spin_box_);

  // buttons frame
  auto* buttons_frame = WidgetCreator::MakeFrame(this);
  settings_frame_layout->addWidget(buttons_frame, 1);
  auto* buttons_frame_layout = new QHBoxLayout(buttons_frame);
  // buttons_frame->setLayout(buttons_frame_layout);
  buttons_frame_layout->addWidget(reset_button_, 1);
  buttons_frame_layout->addWidget(analyze_button_, 1);
}

auto MainWindow::SetupSignalsAndSlots() -> void {
  connect(reset_button_, &QPushButton::clicked, this, &MainWindow::OnButtonResetClicked);
  connect(analyze_button_, &QPushButton::clicked, this, &MainWindow::OnButtonAnalyzeClicked);
}

void MainWindow::ResetValues() {
  for (const auto& subject : db_service_->FetchSubject()) {
    user_desired_subject_combo_box_->addItem(subject);
  }
  user_point_spin_box_->setValue(25.0);
  user_desired_tuition_fees_spin_box_->setValue(7'000'000);
  weight_quota_spin_box_->setValue(0.25);
  weight_rank_spin_box_->setValue(0.25);
  weight_point_spin_box_->setValue(0.25);
  weight_fee_spin_box_->setValue(0.25);
}

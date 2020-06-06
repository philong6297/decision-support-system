//
// Created by longlp on 04/12/2019.
//

#include "decision_table_window.hpp"
#include <QHeaderView>
#include <QLabel>
#include <QLayout>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QTableView>
#include "solver.hpp"

DecisionTableWindow::DecisionTableWindow(
    const dss::InputParams&                         input,
    const std::vector<dss::ReceivedDatabaseRecord>& db_records) :
    QWidget(nullptr),
    user_label_{new QLabel(this)},
    weight_label_{new QLabel(this)},
    vector_label_{new QLabel(this)},
    table_view_(new QTableView(this)),
    model_{new QStandardItemModel(this)} {
  Setup();
  UpdateTable(input, db_records);
}

void DecisionTableWindow::Setup() {
  hide();
  SetupUi();
}

void DecisionTableWindow::SetupUi() {
  setFixedSize(1182 * 3, 750 * 2);

  setWindowTitle("Kết Quả");

  auto* layout = new QGridLayout(this);
  layout->setContentsMargins(9, 9, 9, 9);
  layout->setVerticalSpacing(25);

  // main window label
  auto* main_window_label = new QLabel(this);
  QFont main_window_font("Ubuntu");
  main_window_font.setPointSize(20);
  main_window_label->setFont(main_window_font);
  main_window_label->setText("Bảng Quyết Định");
  main_window_label->setScaledContents(true);
  main_window_label->setWordWrap(true);
  main_window_label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  layout->addWidget(main_window_label, 0, 0, 1, 4);

  // Info weight label
  weight_label_->setTextFormat(Qt::RichText);
  weight_label_->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  layout->addWidget(weight_label_, 1, 0, 1, 1);

  // Info user label
  user_label_->setTextFormat(Qt::RichText);
  user_label_->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  layout->addWidget(user_label_, 1, 1, 1, 1);

  // vector label
  vector_label_->setTextFormat(Qt::RichText);
  vector_label_->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  layout->addWidget(vector_label_, 1, 2, 1, 1);

  layout->addWidget(table_view_, 2, 0, 1, 4);

  layout->setRowStretch(0, 1);
  layout->setRowStretch(1, 2);
  layout->setRowStretch(2, 15);
  table_view_->verticalHeader()->setVisible(false);
  table_view_->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
  // table_view_->horizontalHeader()->setStretchLastSection(true);
  // table_view_->horizontalHeader()->setResizeContentsPrecision(0);
  // table_view_->setFixedSize(this->size());
}

void DecisionTableWindow::showEvent(QShowEvent* event) {
  emit ShowSignal();
  QWidget::showEvent(event);
}
void DecisionTableWindow::closeEvent(QCloseEvent* event) {
  emit CloseSignal();
  QWidget::closeEvent(event);
}
void DecisionTableWindow::UpdateTable(const dss::InputParams&                         input_params,
                                      const std::vector<dss::ReceivedDatabaseRecord>& db_records) {
  const auto& [solve_records, s_plus, s_minus] = dss::solve(input_params, db_records);
  const auto total_rows                        = static_cast<int32_t>(std::size(solve_records));
  model_->clear();
  model_->setRowCount(total_rows);
  for (int32_t row = 0; row < total_rows; ++row) {
    const auto& record = solve_records[static_cast<size_t>(row)];
    model_->setItem(row, 0, new QStandardItem(QString::number(record.performance_score)));
    model_->setItem(row, 1, new QStandardItem(QString::number(record.s_plus_score)));
    model_->setItem(row, 2, new QStandardItem(QString::number(record.s_minus_score)));
    model_->setItem(row, 3, new QStandardItem(record.university_name));
    model_->setItem(row, 4, new QStandardItem(record.major_name));
    model_->setItem(row, 5, new QStandardItem(QString::number(record.university_rank)));
    model_->setItem(row, 6, new QStandardItem(QString::number(record.criteria_rank)));
    model_->setItem(row, 7, new QStandardItem(QString::number(record.fee)));
    model_->setItem(row, 8, new QStandardItem(QString::number(record.criteria_fee)));
    model_->setItem(row, 9, new QStandardItem(QString::number(record.point)));
    model_->setItem(row, 10, new QStandardItem(QString::number(record.criteria_point)));
    model_->setItem(row, 11, new QStandardItem(QString::number(record.quota)));
    model_->setItem(row, 12, new QStandardItem(QString::number(record.criteria_quota)));
  }
  auto* proxy_model = new QSortFilterProxyModel{};
  proxy_model->setSourceModel(model_);
  proxy_model->sort(0, Qt::SortOrder::DescendingOrder);
  table_view_->setModel(proxy_model);

  SetupModel();

  const auto weight_text =
      QString(
          "<html><head/><body><p>Trọng Số Điểm: %1</p><p>Trọng số Chỉ "
          "Tiêu: %2</p><p>Trọng Số Xếp Hạng: %3</p><p>Trọng Số Học Phí: %4</p></body></html>")
          .arg(QString::number(input_params.weight_point))
          .arg(QString::number(input_params.weight_quota))
          .arg(QString::number(input_params.weight_rank))
          .arg(QString::number(input_params.weight_fee));
  weight_label_->setText(weight_text);

  const auto user_text = QString(
                             "<html><head/><body><p>Điểm Thi Người Dùng: %1</p><p>Học phí Mong "
                             "Muốn: %2 VND</p><p>Ngành "
                             "Nguyện Vọng: %3<br/></p></body></html>")
                             .arg(QString::number(input_params.point))
                             .arg(QString::number(input_params.desired_fee))
                             .arg(input_params.subject);
  user_label_->setText(user_text);

  const auto vector_text = QString(
                               "<html><head/><body>"
                               "<p>"
                               "Vector S+: (%1, %2, %3, %4)</p><p>Vector S- : (%5, %6, %7, %8)"
                               "</p>"
                               "<p>Tổng  Số Lựa  Chọn Dựa Trên  Ngành : %9</p>"
                               "</body></html>")
                               .arg(QString::number(s_plus.criteria_point))
                               .arg(QString::number(s_plus.criteria_quota))
                               .arg(QString::number(s_plus.criteria_rank))
                               .arg(QString::number(s_plus.criteria_fee))
                               .arg(QString::number(s_minus.criteria_point))
                               .arg(QString::number(s_minus.criteria_quota))
                               .arg(QString::number(s_minus.criteria_rank))
                               .arg(QString::number(s_minus.criteria_fee))
                               .arg(QString::number(db_records.size()));
  vector_label_->setText(vector_text);
}
void DecisionTableWindow::SetupModel() {
  model_->setColumnCount(13);
  model_->setHorizontalHeaderLabels({"P",
                                     "S+",
                                     "S-",
                                     "Trường",
                                     "Ngành",
                                     "Xếp Hạng",
                                     "Chỉ số  Xếp Hạng",
                                     "Học Phí (VND)",
                                     "Chỉ số Học Phí",
                                     "Điểm Chuẩn",
                                     "Chỉ số Điểm",
                                     "Chỉ Tiêu",
                                     "Chỉ số Chỉ Tiêu"});
  const auto width = this->width();
  table_view_->setColumnWidth(0, width * 2 / 28);
  table_view_->setColumnWidth(1, width * 2 / 28);
  table_view_->setColumnWidth(2, width * 2 / 28);
  table_view_->setColumnWidth(3, width * 3 / 28);
  table_view_->setColumnWidth(4, width * 3 / 28);
  table_view_->setColumnWidth(5, width * 2 / 28);
  table_view_->setColumnWidth(6, width * 2 / 28);
  table_view_->setColumnWidth(7, width * 2 / 28);
  table_view_->setColumnWidth(8, width * 2 / 28);
  table_view_->setColumnWidth(9, width * 2 / 28);
  table_view_->setColumnWidth(10, width * 2 / 28);
  table_view_->setColumnWidth(11, width * 2 / 28);
  table_view_->setColumnWidth(12, width * 2 / 28);
}

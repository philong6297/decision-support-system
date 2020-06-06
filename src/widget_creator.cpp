//
// Created by longlp on 10/12/2019.
//

#include "widget_creator.hpp"

auto WidgetCreator::MakePushButton(QWidget* parent, const QString& title, bool is_default)
    -> QPushButton* {
  auto* button = new QPushButton(parent);
  button->setDefault(is_default);
  button->setText(title);
  return button;
}

auto WidgetCreator::MakeDoubleSpinBox(QWidget*       parent,
                                      const QString& suffix,
                                      double         minimum,
                                      double         maximum,
                                      double         step) -> QDoubleSpinBox* {
  auto* spin_box = new QDoubleSpinBox(parent);
  spin_box->setSuffix(suffix);
  spin_box->setMaximum(maximum);
  spin_box->setMinimum(minimum);
  spin_box->setSingleStep(step);
  spin_box->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  return spin_box;
}

auto WidgetCreator::MakeSpinBox(QWidget*       parent,
                                const QString& suffix,
                                int32_t        minimum,
                                int32_t        maximum,
                                int32_t        step) -> QSpinBox* {
  auto* spin_box = new QSpinBox(parent);
  spin_box->setSuffix(suffix);
  spin_box->setMaximum(maximum);
  spin_box->setMinimum(minimum);
  spin_box->setSingleStep(step);
  spin_box->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  return spin_box;
}

auto WidgetCreator::MakeGroupBox(QWidget* parent, const QString& title) -> QGroupBox* {
  auto* group_box = new QGroupBox(parent);
  group_box->setTitle(title);
  group_box->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  group_box->setFlat(true);
  return group_box;
}

auto WidgetCreator::MakeFrame(QWidget* parent) -> QFrame* {
  auto* frame = new QFrame(parent);
  frame->setFrameShape(QFrame::NoFrame);
  frame->setFrameShadow(QFrame::Sunken);
  return frame;
}

auto WidgetCreator::MakeComboBox(QWidget* parent) -> QComboBox* {
  auto* combo_box = new QComboBox(parent);
  combo_box->setEditable(true);
  return combo_box;
}

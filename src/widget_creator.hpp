//
// Created by longlp on 10/12/2019.
//

#ifndef SRC_WIDGET_CREATOR_HPP_
#define SRC_WIDGET_CREATOR_HPP_
#include <QtWidgets>
struct WidgetCreator {
  [[nodiscard]] static auto MakePushButton(QWidget* parent, const QString& title, bool is_default)
      -> QPushButton*;

  [[nodiscard]] static auto MakeDoubleSpinBox(QWidget*       parent,
                                              const QString& suffix,
                                              double         minimum,
                                              double         maximum,
                                              double         step) -> QDoubleSpinBox*;

  [[nodiscard]] static auto MakeSpinBox(QWidget*       parent,
                                        const QString& suffix,
                                        int32_t        minimum,
                                        int32_t        maximum,
                                        int32_t        step) -> QSpinBox*;

  [[nodiscard]] static auto MakeGroupBox(QWidget* parent, const QString& title) -> QGroupBox*;

  [[nodiscard]] static auto MakeFrame(QWidget* parent) -> QFrame*;

  [[nodiscard]] static auto MakeComboBox(QWidget* parent) -> QComboBox*;
};

#endif  // SRC_WIDGET_CREATOR_HPP_

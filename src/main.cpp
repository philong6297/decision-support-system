//
// Created by longlp on 03/12/2019.
//
#include <QApplication>
#include "main_window.hpp"
int main(int argc, char** argv) {
  QApplication app(argc, argv);
  MainWindow   main_window;
  main_window.show();
  return QApplication::exec();
}

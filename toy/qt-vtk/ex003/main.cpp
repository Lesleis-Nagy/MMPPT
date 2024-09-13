//
// Created by Lesleis Nagy on 23/07/2024.
//

#include <QApplication>
#include <QVTKOpenGLWindow.h>

#include "MainWindow.hpp"

int main(int argc, char *argv[]) {

  QSurfaceFormat::setDefaultFormat(QVTKOpenGLWindow::defaultFormat());

  QApplication app(argc, argv);

  MainWindow main_window;
  main_window.show();

  return QApplication::exec();

}

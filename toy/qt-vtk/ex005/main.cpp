//
// Created by Lesleis Nagy on 23/07/2024.
//

#include <QApplication>
#include <QVTKOpenGLWindow.h>
#include <QSettings>

#include "main_window.hpp"

#define APP_ORG_NAME "lesleisnagy.org"
#define APP_NAME "mmffp"

int main(int argc, char *argv[]) {

  QSurfaceFormat::setDefaultFormat(QVTKOpenGLWindow::defaultFormat());

  QApplication app(argc, argv);

  QSettings::setDefaultFormat(QSettings::IniFormat);
  QApplication::setOrganizationName(APP_ORG_NAME);
  QApplication::setApplicationName(APP_NAME);

  main_window main_window;
  main_window.show();

  return QApplication::exec();

}

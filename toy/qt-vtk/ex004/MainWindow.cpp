//
// Created by Lesleis Nagy on 23/07/2024.
//

#include <iostream>

#include <QVTKOpenGLNativeWidget.h>
#include <QRegularExpression>

#include "MainWindow.hpp"

main_window::main_window() {

  this->setupUi(this);

  connect(_btn_draw, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_draw_clicked()));

}

main_window::~main_window() = default;

void main_window::slot_btn_draw_clicked() {

  std::cout << "slot_btn_draw_clicked()" << std::endl;

  draw_plot();

}

void
main_window::draw_plot() {

}

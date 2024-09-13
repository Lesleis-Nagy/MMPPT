//
// Created by Lesleis Nagy on 23/07/2024.
//

#include <iostream>

#include <QVTKOpenGLNativeWidget.h>
#include <QRegularExpression>

#include "MainWindow.hpp"

MainWindow::MainWindow() {

  this->setupUi(this);

  connect(_btn_draw, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_draw_clicked()));

}

MainWindow::~MainWindow() = default;

void MainWindow::slot_btn_draw_clicked() {

  std::cout << "slot_btn_draw_clicked()" << std::endl;

  draw_plot();

}

void
MainWindow::draw_plot() {

}

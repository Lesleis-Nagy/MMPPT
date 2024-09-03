//
// Created by Lesleis Nagy on 23/07/2024.
//

#ifndef VCOMPARE_TOY_QT_VTK_MAINWINDOWS_HPP_
#define VCOMPARE_TOY_QT_VTK_MAINWINDOWS_HPP_

#include <QMainWindow>
#include <QPointer>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QGraphicsScene>

#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

#include "ui_MainWindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindow {
  Q_OBJECT

 public:

  MainWindow();
  ~MainWindow();

 public slots:

  void
  slot_btn_draw_clicked();

 private:

  void
  draw_plot();

};

#endif // VCOMPARE_TOY_QT_VTK_MAINWINDOWS_HPP_

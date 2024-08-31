//
// Created by Lesleis Nagy on 23/07/2024.
//

#ifndef VCOMPARE_TOY_QT_VTK_MAINWINDOWS_HPP_
#define VCOMPARE_TOY_QT_VTK_MAINWINDOWS_HPP_

#include <QMainWindow>

#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

#include "ui_MainWindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindow {
  Q_OBJECT

 public:

  MainWindow();
  ~MainWindow() override = default;

 public slots:

  void slot_btn_process_clicked();
  void slot_btn_cylinder_clicked();
  void slot_btn_cube_clicked();
  void slot_btn_clear_clicked();

 private:

  double _bg_red;
  double _bg_green;
  double _bg_blue;

  double _actor_red;
  double _actor_green;
  double _actor_blue;

  vtkSmartPointer<vtkRenderer> _renderer;

  vtkSmartPointer<vtkActor> _current_actor;

  std::vector<vtkSmartPointer<vtkActor>> _four_corners;

};

#endif // VCOMPARE_TOY_QT_VTK_MAINWINDOWS_HPP_

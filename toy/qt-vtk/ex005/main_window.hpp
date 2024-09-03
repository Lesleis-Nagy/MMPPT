//
// Created by Lesleis Nagy on 23/07/2024.
//

#ifndef MMPPT_TOY_QT_VTK_EX005_MAIN_WINDOW_HPP_
#define MMPPT_TOY_QT_VTK_EX005_MAIN_WINDOW_HPP_

#include <QMainWindow>

#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

#include "ui_main_window.h"

class main_window : public QMainWindow, private Ui::MainWindow {
  Q_OBJECT

 public:

  main_window();
  ~main_window() override = default;

 public slots:

  void slot_btn_load_tecplot_clicked();
  void slot_btn_clear_clicked();
  void slot_btn_mfm_clicked();
  void slot_btn_holography_clicked();

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

#endif // MMPPT_TOY_QT_VTK_EX005_MAIN_WINDOW_HPP_

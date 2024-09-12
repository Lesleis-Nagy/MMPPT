//
// Created by Lesleis Nagy on 23/07/2024.
//

#ifndef MMPPT_TOY_QT_VTK_EX005_MAIN_WINDOW_HPP_
#define MMPPT_TOY_QT_VTK_EX005_MAIN_WINDOW_HPP_

#include <QMainWindow>
#include <QSettings>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkObjectFactory.h>
#include "plane_widget.h"

#include "ui_main_window.h"

#include "model.hpp"

namespace {

// Handle mouse events.
class TrackballInteractor : public vtkInteractorStyleTrackballActor {

 public:

  static TrackballInteractor *New();

 vtkTypeMacro(TrackballInteractor, vtkInteractorStyleTrackballActor);

  void OnLeftButtonDown() override {

    vtkInteractorStyleTrackballActor::OnLeftButtonDown();

    if (this->InteractionProp == this->_actor) {
      std::cout << "the actor is selected." << std::endl;
    } else {
      std::cout << "Doing nothing." << std::endl;
    }

  }

  void OnKeyPress() override {
    char key = this->Interactor->GetKeyCode();
    switch (key) {
      case 's':std::cout << "S is pressed" << std::endl;
        break;
      default:break;
    }
    vtkInteractorStyleTrackballActor::OnKeyPress();
  }

  void set_actor(vtkActor *actor) {
    _actor = actor;
  }

 private:
  vtkActor *_actor;

};

vtkStandardNewMacro(TrackballInteractor);

}

class main_window : public QMainWindow, private Ui::MainWindow {
 Q_OBJECT

 public:

  main_window();
  ~main_window() override = default;

 public slots:

  void slot_btn_load_tecplot_clicked();
  void slot_btn_sample_points_show_clicked();
  void slot_btn_sample_points_clear_clicked();
  void slot_btn_camera_dolly_in_clicked();
  void slot_btn_camera_dolly_out_clicked();

  void slot_btn_mfm_clicked();
  void slot_btn_holography_clicked();
  void slot_btn_save_image_clicked();

  void slot_hsl_sample_points_scale_changed(int);
  void slot_txt_sample_points_scale_changed(QString);

  void slot_txt_nx_text_changed(QString);

 private:

  QRegularExpression _regex_int{R"([0-9]+)"};
  QRegularExpression _regex_three_dp_float{R"(\.[0-9]{2}[1-9])"};

  QRegularExpressionValidator _int_validator{_regex_int};
  QRegularExpressionValidator _three_dp_float_validator{_regex_three_dp_float};

  vtkSmartPointer<vtkRenderer> _renderer;

  vtkSmartPointer<vtkActor> _current_actor;

  vtkSmartPointer<PlaneWidget> _plane_widget;

  vtkSmartPointer<TrackballInteractor> _interactor;

  std::vector<vtkSmartPointer<vtkActor>> _world_sample_points;

  std::optional<Model> _model;




  void
  create_world_sample_points();

  void
  destroy_world_sample_points();

};

#endif // MMPPT_TOY_QT_VTK_EX005_MAIN_WINDOW_HPP_

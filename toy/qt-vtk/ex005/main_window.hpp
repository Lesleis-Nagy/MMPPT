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

    if (this->InteractionProp == this->cylinder) {
      std::cout << "Picked cylinder." << std::endl;
    } else if (this->InteractionProp == this->plane) {
      std::cout << "Picked plane." << std::endl;
    }
  }

  void OnLeftButtonUp() override {

    vtkInteractorStyleTrackballActor::OnLeftButtonUp();

    if (this->InteractionProp == this->cylinder) {
      std::cout << "Released cylinder." << std::endl;
    } else if (this->InteractionProp == this->plane) {
      std::cout << "Released plane." << std::endl;
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

  vtkSmartPointer<vtkActor> cylinder;
  vtkSmartPointer<vtkActor> plane;

};

vtkStandardNewMacro(TrackballInteractor);

}

class main_window : public QMainWindow, private Ui::MainWindow {
 Q_OBJECT

 public:

  // Aliases
  using PtrRenderer = vtkSmartPointer<vtkRenderer>;
  using PtrActor = vtkSmartPointer<vtkActor>;
  using PtrTrackballInteractor = vtkSmartPointer<TrackballInteractor>;

  main_window();
  ~main_window() override = default;

 public slots:

  void slot_timer_timeout();
  void slot_btn_load_tecplot_clicked();
  void slot_btn_clear_clicked();
  void slot_btn_mfm_clicked();
  void slot_btn_holography_clicked();
  void slot_btn_save_image_clicked();

  void slot_txt_nx_text_changed(const QString &);

 private:

  // Create the settings menu
  QMenu* _settingsMenu;

  // Add actions to the settings menu
  QAction* _preferencesAction;
  QAction* _aboutAction;

  QRegularExpression _regex_int{R"([0-9]+)"};
  QRegularExpressionValidator _int_validator{_regex_int};

  PtrRenderer _renderer;

  PtrActor _current_actor;
  PtrActor _plane_actor;
  PtrActor _plane_origin_actor;
  PtrActor _plane_point1_actor;
  PtrActor _plane_point2_actor;

  PtrTrackballInteractor _interactor;

  std::optional<Model> _model;

};

#endif // MMPPT_TOY_QT_VTK_EX005_MAIN_WINDOW_HPP_

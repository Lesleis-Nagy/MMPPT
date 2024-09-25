//
// Created by Lesleis Nagy on 23/07/2024.
//

#ifndef MMPPT_TOY_QT_VTK_EX005_MAIN_WINDOW_HPP_
#define MMPPT_TOY_QT_VTK_EX005_MAIN_WINDOW_HPP_

#include <iostream>

#include <QErrorMessage>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSettings>
#include <QtCharts/QChart>
#include <QVTKOpenGLNativeWidget.h>

#include <vtkCamera.h>
#include <vtkCoordinate.h>
#include <vtkCylinderSource.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkObjectFactory.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkTransform.h>

#include "ui_main_window.h"

#include "config_consts.h"
#include "load_tecplot.hpp"
#include "model.hpp"
#include "preferences_dialog.hpp"
#include "sample_plane.hpp"

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

class MainWindow : public QMainWindow, private Ui::MainWindow {

 Q_OBJECT

 public:

  // Aliases
  //using PtrRenderer = vtkSmartPointer<vtkRenderer>;
  //using PtrActor = vtkSmartPointer<vtkActor>;
  //using PtrTrackballInteractor = vtkSmartPointer<TrackballInteractor>;

  MainWindow();
  ~MainWindow() override = default;

 public slots:

  void slot_timer_timeout();

  void slot_btn_load_tecplot_clicked();
  void slot_btn_clear_clicked();
  void slot_btn_set_arrow_scale_clicked();
  void slot_btn_camera_x_clicked();
  void slot_btn_camera_y_clicked();
  void slot_btn_camera_z_clicked();
  void slot_chk_ugrid_changed(Qt::CheckState state);
  void slot_chk_vectors_changed(Qt::CheckState state);
  void slot_sli_ugrid_opacity_changed(int value);
  void slot_sli_vector_opacity_changed(int value);

  void slot_sli_plane_polar_angle_changed(int value);
  void slot_txt_plane_polar_angle_changed(QString value);
  void slot_sli_plane_azimuthal_angle_changed(int value);
  void slot_txt_plane_azimuthal_angle_changed(QString value);
  void slot_sli_plane_orientation_angle_changed(int value);
  void slot_txt_plane_orientation_angle_changed(QString value);
  void slot_txt_plane_target_x_changed(QString value);
  void slot_txt_plane_target_y_changed(QString value);
  void slot_txt_plane_target_z_changed(QString value);
  void slot_txt_plane_width_changed(QString value);
  void slot_txt_plane_distance_changed(QString value);
  void slot_chk_plane_show_reference_points_changed(Qt::CheckState value);
  void slot_chk_plane_hide_plane_changed(Qt::CheckState value);


  void slot_btn_mfm_clicked();
  void slot_btn_holography_clicked();
  void slot_btn_save_image_clicked();

  void slot_menu_preferences();

 private:

  // Create the settings menu
  QMenu* _settingsMenu;

  // Add actions to the settings menu
  QAction* _preferencesAction;
  QAction* _aboutAction;

  QRegularExpression _regex_float{R"([-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?)"};
  QRegularExpression _regex_int{R"([0-9]+)"};
  QRegularExpressionValidator _float_validator{_regex_int};
  QRegularExpressionValidator _int_validator{_regex_int};

  vtkSmartPointer<vtkRenderer> _renderer;

  vtkSmartPointer<TrackballInteractor> _interactor;

  bool _model_ugrid_actor_showing{false};
  bool _model_arrow_actor_showing{false};

  std::optional<Model> _model;
  std::optional<SamplePlane> _sample_plane;

  //--------------------------------------------------------------------------

  [[nodiscard]] bool
  is_float(const QString &str) const;

  [[nodiscard]] bool
  is_int(const QString &str) const;

  void
  clear_model();

  void
  hide_ugrid_actor();

  void
  show_ugrid_actor();

  void
  hide_arrow_actor();

  void
  show_arrow_actor();

  void
  set_ugrid_opacity(double);

  void
  set_arrow_opacity(double);

  void
  set_arrow_scale(double);

  void
  set_camera_to_x_pos();

  void
  set_camera_to_y_pos();

  void
  set_camera_to_z_pos();

  [[nodiscard]] double
  plane_polar_angle_from_gui() const;

  [[nodiscard]] double
  plane_azimuthal_angle_from_gui() const;

  [[nodiscard]] double
  plane_orientation_angle_from_gui() const;

  [[nodiscard]] std::optional<double>
  plane_target_x_from_gui() const;

  [[nodiscard]] std::optional<double>
  plane_target_y_from_gui() const;

  [[nodiscard]] std::optional<double>
  plane_target_z_from_gui() const;

  [[nodiscard]] std::optional<double>
  plane_width_from_gui() const;

  [[nodiscard]] std::optional<double>
  plane_height_from_gui() const;

  [[nodiscard]] std::optional<double>
  plane_distance_from_gui() const;

  void
  initialize_gui_plane_parameters_from_model();

  bool
  is_plane_hidden();

  void
  hide_plane();

  void
  show_plane();

};

#endif // MMPPT_TOY_QT_VTK_EX005_MAIN_WINDOW_HPP_

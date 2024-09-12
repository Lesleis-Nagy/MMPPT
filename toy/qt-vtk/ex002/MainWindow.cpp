//
// Created by Lesleis Nagy on 23/07/2024.
//

#include <iostream>

#include <QVTKOpenGLNativeWidget.h>

#include "MainWindow.hpp"

main_window::main_window() : _bg_red(0.0), _bg_green(0.0), _bg_blue(0.0),
                             _actor_red(1.0), _actor_green(1.0),
                             _actor_blue(1.0) {

  this->setupUi(this);
  _timer = new QTimer(this);
  _timer->setInterval(10);

  display_camera_mode();

  // Create and assign a new vtkRenderer.
  _renderer = vtkSmartPointer<vtkRenderer>::New();
  _renderer->SetBackground(_bg_red, _bg_green, _bg_blue);
  _renderer->ResetCamera();
  _vtk_widget->renderWindow()->AddRenderer(_renderer);

  // Set the _cylinder_actor.
  PtrCylinderSource cylinder_source = PtrCylinderSource::New();
  cylinder_source->SetResolution(300);
  cylinder_source->Update();

  _cylinder_poly_data_mapper = PtrPolyDataMapper::New();
  _cylinder_poly_data_mapper->SetInputConnection(
      cylinder_source->GetOutputPort()
  );

  _cylinder_actor = PtrActor::New();
  _cylinder_actor->SetMapper(_cylinder_poly_data_mapper);
  _cylinder_actor->GetProperty()->SetColor(_actor_red,
                                           _actor_green,
                                           _actor_blue);

  // Set the plane _plane_actor.
  _plane_source = PtrPlaneSource::New();
  _plane_source->SetResolution(10, 10);
  _plane_source->SetCenter(0, 0, 1);
  _plane_source->SetNormal(0, 0, 1);
  _plane_source->Update();

  _sphere_plane_origin = PtrSphereSource::New();
  _sphere_plane_origin->SetRadius(0.1);
  _sphere_plane_origin->SetPhiResolution(100);
  _sphere_plane_origin->SetThetaResolution(100);

  _sphere_plane_point1 = PtrSphereSource::New();
  _sphere_plane_point1->SetRadius(0.1);
  _sphere_plane_point1->SetPhiResolution(100);
  _sphere_plane_point1->SetThetaResolution(100);

  _sphere_plane_point2 = PtrSphereSource::New();
  _sphere_plane_point2->SetRadius(0.1);
  _sphere_plane_point2->SetPhiResolution(100);
  _sphere_plane_point2->SetThetaResolution(100);

  _plane_poly_data_mapper = PtrPolyDataMapper::New();
  _plane_poly_data_mapper->SetInputConnection(_plane_source->GetOutputPort());

  _sphere_plane_origin_data_mapper = PtrPolyDataMapper::New();
  _sphere_plane_origin_data_mapper->SetInputConnection(_sphere_plane_origin->GetOutputPort());

  _sphere_plane_point1_data_mapper = PtrPolyDataMapper::New();
  _sphere_plane_point1_data_mapper->SetInputConnection(_sphere_plane_point1->GetOutputPort());

  _sphere_plane_point2_data_mapper = PtrPolyDataMapper::New();
  _sphere_plane_point2_data_mapper->SetInputConnection(_sphere_plane_point2->GetOutputPort());

  _plane_actor = PtrActor::New();
  _plane_actor->SetMapper(_plane_poly_data_mapper);
  _plane_actor->GetProperty()->SetColor(_actor_red, _actor_green, _actor_blue);
  _plane_actor->GetProperty()->LightingOff();

  _sphere_plane_origin_actor = PtrActor::New();
  _sphere_plane_origin_actor->SetMapper(_sphere_plane_origin_data_mapper);
  _sphere_plane_origin_actor->GetProperty()->SetColor(1.0, 0.0, 0.0);

  _sphere_plane_point1_actor = PtrActor::New();
  _sphere_plane_point1_actor->SetMapper(_sphere_plane_point1_data_mapper);
  _sphere_plane_point1_actor->GetProperty()->SetColor(0.0, 1.0, 0.0);

  _sphere_plane_point2_actor = PtrActor::New();
  _sphere_plane_point2_actor->SetMapper(_sphere_plane_point2_data_mapper);
  _sphere_plane_point2_actor->GetProperty()->SetColor(0.0, 0.0, 1.0);

  _object_interaction_style = PtrMouseInteractorStyle5::New();
  _object_interaction_style->SetDefaultRenderer(_renderer);
  _object_interaction_style->cylinder = _cylinder_actor;
  _object_interaction_style->plane = _plane_actor;

  _world_interaction_style = PtrTrackballCameraStyle::New();
  _world_interaction_style->SetDefaultRenderer(_renderer);

  _vtk_widget->renderWindow()->GetInteractor()->SetInteractorStyle(
      _world_interaction_style);

  _renderer->AddActor(_cylinder_actor);

  _renderer->AddActor(_plane_actor);
  _renderer->AddActor(_sphere_plane_origin_actor);
  _renderer->AddActor(_sphere_plane_point1_actor);
  _renderer->AddActor(_sphere_plane_point2_actor);

  _renderer->ResetCamera();
  _renderer->GetActiveCamera()->SetPosition(7 * cos(45.0 * M_PI / 180.0), 1,
                                            7 * sin(45.0 * M_PI / 180.0));
  _renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);

  _vtk_widget->update();
  _vtk_widget->renderWindow()->Render();


  // Hook up signals and slots.

  connect(_btn_toggle, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_toggle_clicked()));

  connect(_timer, &QTimer::timeout,
          this, &main_window::slot_timer_timeout);

  _timer->start();

}

void
main_window::slot_timer_timeout() {

  double ori[4] = {1.0, 1.0, 1.0, 1.0};
  double pt1[4] = {1.0, 1.0, 1.0, 1.0};;
  double pt2[4] = {1.0, 1.0, 1.0, 1.0};;

  double w_ori[4] = {0.0, 0.0, 0.0, 0.0};
  double w_pt1[4] = {0.0, 0.0, 0.0, 0.0};
  double w_pt2[4] = {0.0, 0.0, 0.0, 0.0};

  _plane_source->GetOrigin(ori);
  _plane_source->GetPoint1(pt1);
  _plane_source->GetPoint2(pt2);

  vtkMatrix4x4 *pam = _plane_actor->GetMatrix();

  pam->MultiplyPoint(ori, w_ori);
  pam->MultiplyPoint(pt1, w_pt1);
  pam->MultiplyPoint(pt2, w_pt2);

  std::cout << "Plane Origin: ";
  std::cout << ori[0] << ", " << ori[1] << ", " << ori[2] << ", " << ori[3] << std::endl;
  std::cout << "World Origin: ";
  std::cout << w_ori[0] << ", " << w_ori[1] << ", " << w_ori[2] << ", " << w_ori[3] << std::endl;

  std::cout << "Plane Point1: ";
  std::cout << pt1[0] << ", " << pt1[1] << ", " << pt1[2] << ", " << pt1[3] << std::endl;
  std::cout << "World Point1: ";
  std::cout << w_pt1[0] << ", " << w_pt1[1] << ", " << w_pt1[2] << ", " << w_pt1[3]  << std::endl;

  std::cout << "Plane Point2: ";
  std::cout << pt2[0] << ", " << pt2[1] << ", " << pt2[2] << ", " << pt2[3]  << std::endl;
  std::cout << "World Point2: ";
  std::cout << w_pt2[0] << ", " << w_pt2[1] << ", " << w_pt2[2] << ", " << w_pt2[3]  << std::endl;

  double angle = 0.1;

  vtkVector3d axis{0.0, 0.0, 0.0};

  vtkVector3d v_ori{w_ori};
  vtkVector3d v_pt1{w_pt1};
  vtkVector3d v_pt2{w_pt2};

  vtkVector3d e1 = (v_pt1 - v_ori).Normalized();
  vtkVector3d e2 = (v_pt2 - v_ori).Normalized();

  vtkVector3d ax = e1.Cross(e2).Normalized();

  _plane_actor->RotateWXYZ(angle, ax.GetX(), ax.GetY(), ax.GetZ());

  double bounds[6];
  _plane_actor->GetBounds(bounds);

  std::cout << "xmin: " << bounds[0] << std::endl;
  std::cout << "xmax: " << bounds[1] << std::endl;
  std::cout << "ymin: " << bounds[2] << std::endl;
  std::cout << "ymax: " << bounds[3] << std::endl;
  std::cout << "zmin: " << bounds[4] << std::endl;
  std::cout << "zmax: " << bounds[5] << std::endl;

  _sphere_plane_origin_actor->SetPosition(w_ori);
  _sphere_plane_point1_actor->SetPosition(w_pt1);
  _sphere_plane_point2_actor->SetPosition(w_pt2);

  _vtk_widget->update();
  _vtk_widget->renderWindow()->Render();

}

void
main_window::slot_btn_toggle_clicked() {
  if (_camera_mode == WORLD_MODE) {
    _camera_mode = OBJECT_MODE;
    _vtk_widget->renderWindow()->GetInteractor()->SetInteractorStyle(
        _object_interaction_style);
  } else {
    _camera_mode = WORLD_MODE;
    _vtk_widget->renderWindow()->GetInteractor()->SetInteractorStyle(
        _world_interaction_style);
  }
  display_camera_mode();
}

void
main_window::display_camera_mode() {
  switch (_camera_mode) {
    case OBJECT_MODE:_status_bar->showMessage("Object mode");
      break;
    case WORLD_MODE:_status_bar->showMessage("World mode");
      break;
  }
}

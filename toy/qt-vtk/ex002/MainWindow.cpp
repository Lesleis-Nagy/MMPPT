//
// Created by Lesleis Nagy on 23/07/2024.
//

#include <iostream>

#include <QVTKOpenGLNativeWidget.h>

#include "MainWindow.hpp"

MainWindow::MainWindow() : _bg_red(0.0), _bg_green(0.0), _bg_blue(0.0),
                           _actor_red(1.0), _actor_green(1.0),
                           _actor_blue(1.0) {
  this->setupUi(this);

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
    cylinder_source->GetOutputPort());

  _cylinder_actor = PtrActor::New();
  _cylinder_actor->SetMapper(_cylinder_poly_data_mapper);
  _cylinder_actor->GetProperty()->SetColor(_actor_red, _actor_green,
                                           _actor_blue);
  _cylinder_actor->RotateX(30.0);
  _cylinder_actor->RotateY(-45.0);

  // Set the plane _plane_actor.
  PtrPlaneSource plane_source = PtrPlaneSource::New();
  plane_source->SetResolution(10, 10);
  //plane_source->SetOrigin(0, 0, 0);
  //plane_source->SetPoint1( 0, 1, 0 );
  //plane_source->SetPoint2( 1, 0, 0 );
  plane_source->SetCenter(1, 1, 1);
  plane_source->SetNormal(1, 1, 0);
  plane_source->Update();

  _plane_poly_data_mapper = PtrPolyDataMapper::New();
  _plane_poly_data_mapper->SetInputConnection(plane_source->GetOutputPort());

  _plane_actor = PtrActor::New();
  _plane_actor->SetMapper(_plane_poly_data_mapper);
  _plane_actor->GetProperty()->SetColor(_actor_red, _actor_green, _actor_blue);
  _plane_actor->GetProperty()->LightingOff();

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


  _renderer->ResetCamera();
  _renderer->GetActiveCamera()->SetPosition(7 * cos(45.0 * M_PI / 180.0), 1,
                                            7 * sin(45.0 * M_PI / 180.0));
  _renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);

  _vtk_widget->update();
  _vtk_widget->renderWindow()->Render();


  // Hook up signals and slots.

  connect(_btn_toggle, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_toggle_clicked()));
}

void
MainWindow::slot_btn_toggle_clicked() {
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
MainWindow::display_camera_mode() {
  switch (_camera_mode) {
    case OBJECT_MODE:
      _status_bar->showMessage("Object mode");
      break;
    case WORLD_MODE:
      _status_bar->showMessage("World mode");
      break;
  }
}

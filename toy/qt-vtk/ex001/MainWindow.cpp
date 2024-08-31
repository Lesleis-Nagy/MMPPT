//
// Created by Lesleis Nagy on 23/07/2024.
//

#include <iostream>

#include <QVTKOpenGLNativeWidget.h>

#include <vtkPolyDataMapper.h>
#include <vtkCubeSource.h>
#include "vtkCylinderSource.h"
#include "vtkSphereSource.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkCoordinate.h"

#include "MainWindow.hpp"

MainWindow::MainWindow() :
_bg_red(0.0), _bg_green(0.0), _bg_blue(0.0),
_actor_red(1.0), _actor_green(1.0), _actor_blue(1.0) {

  this->setupUi(this);

  // Create and assign a new vtkRenderer.
  _renderer = vtkSmartPointer<vtkRenderer>::New();
  _renderer->SetBackground(_bg_red, _bg_green, _bg_blue);
  _renderer->ResetCamera();
  _vtk_widget->renderWindow()->AddRenderer(_renderer);

  // Set the _current_actor to nullptr.
  _current_actor = nullptr;

  // Connect signals and slots.
  connect(_btn_process, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_process_clicked()));
  connect(_btn_cylinder, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_cylinder_clicked()));
  connect(_btn_clear, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_clear_clicked()));
  connect(_btn_cube, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_cube_clicked()));

}

void MainWindow::slot_btn_process_clicked() {

  if (!_four_corners.empty()) {
    for (const auto& actor : _four_corners) {
      _renderer->RemoveActor(actor);
    }
    _four_corners.erase(_four_corners.begin(), _four_corners.end());
  }

  auto width = (double)_vtk_widget->width();
  auto height = (double)_vtk_widget->height();

  std::array<double, 3> bottom_left_corner = {0.0, 0.0, 0.0};
  std::array<double, 3> bottom_right_corner = {2*width, 0.0, 0.0};
  std::array<double, 3> top_right_corner = {2*width, 2*height, 0.0};
  std::array<double, 3> top_left_corner = {0.0, 2*height, 0.0};

  std::array<double, 3> bottom_left_color = {1.0, 0.0, 0.0};
  std::array<double, 3> bottom_right_color = {0.0, 1.0, 0.0};
  std::array<double, 3> top_right_color = {0.0, 0.0, 1.0};
  std::array<double, 3> top_left_color = {1.0, 1.0, 1.0};

  std::array<std::array<double, 3>, 4> corners {
      bottom_left_corner,
      bottom_right_corner,
      top_right_corner,
      top_left_corner
  };

  std::array<std::array<double, 3>, 4> colors {
      bottom_left_color,
      bottom_right_color,
      top_right_color,
      top_left_color
  };

  vtkSmartPointer<vtkCoordinate> coordinate = vtkCoordinate::New();
  coordinate->SetCoordinateSystemToDisplay();

  std::array<double, 3> wc_tl = {0.0, 0.0, 0.0};
  std::array<double, 3> wc_br = {0.0, 0.0, 0.0};
  double *res;

  coordinate->SetValue(
      bottom_left_corner[0],
      bottom_left_corner[1],
      bottom_left_corner[2]
  );
  res = coordinate->GetComputedWorldValue(_renderer);
  std::copy(res, res+3, wc_tl.begin());
  std::cout << "wc_tl: " << wc_tl[0] << ", " << wc_tl[1] << ", " << wc_tl[2] << std::endl;

  coordinate->SetValue(
      top_right_corner[0],
      top_right_corner[1],
      top_right_corner[2]
  );
  res = coordinate->GetComputedWorldValue(_renderer);
  std::copy(res, res+3, wc_br.begin());
  std::cout << "wc_br: " << wc_br[0] << ", " << wc_br[1] << ", " << wc_br[2] << std::endl;

  std::cout << "wc_tl[0] - wc_br[0] = " << wc_tl[0] - wc_br[0] << std::endl;
  std::cout << "wc_tl[1] - wc_br[1] = " << wc_tl[1] - wc_br[1] << std::endl;
  std::cout << "wc_tl[2] - wc_br[2] = " << wc_tl[2] - wc_br[2] << std::endl;

  double xl2 = pow(wc_tl[0] - wc_br[0], 2.0);
  std::cout << "xl2: " << xl2 << std::endl;
  double yl2 = pow(wc_tl[1] - wc_br[1], 2.0);
  std::cout << "yl2: " << yl2 << std::endl;
  double zl2 = pow(wc_tl[2] - wc_br[2], 2.0);
  std::cout << "zl2: " << zl2 << std::endl;

  double len = sqrt(xl2 + yl2 + zl2);

  std::cout << "len: " << len << std::endl;

  for (int i = 0; i < 4; ++i) {
    auto c = corners[i];
    auto color = colors[i];
    coordinate->SetValue(c[0], c[1], c[2]);
    std::array<double, 3> wc = {0.0, 0.0, 0.0};
    res = coordinate->GetComputedWorldValue(_renderer);
    std::copy(res, res+3, wc.begin());

    std::cout << "World equivalent of ("
              << c[0]  << ", " << c[1]  << ", " << c[2]  << "): "
              << wc[0] << ", " << wc[1] << ", " << wc[2] << ")"
              << std::endl;

    vtkSmartPointer<vtkSphereSource> sphere_src = vtkSphereSource::New();
    sphere_src->SetPhiResolution(50);
    sphere_src->SetThetaResolution(50);
    sphere_src->SetRadius(len*0.01);

    vtkSmartPointer<vtkPolyDataMapper> sphere_map = vtkPolyDataMapper::New();
    sphere_map->SetInputConnection(sphere_src->GetOutputPort());

    vtkSmartPointer<vtkActor> sphere_act = vtkActor::New();
    sphere_act->SetMapper(sphere_map);
    sphere_act->SetPosition(wc[0], wc[1], wc[2]);
    sphere_act->GetProperty()->SetColor(color[0], color[1], color[2]);

    _four_corners.push_back(sphere_act);

    _renderer->AddActor(sphere_act);
  }

  _vtk_widget->update();
  _vtk_widget->renderWindow()->Render();

}

void MainWindow::slot_btn_clear_clicked() {

  std::cout << "slot_btn_clear_clicked()" << std::endl;

  if (_current_actor.Get() != nullptr) {

    _renderer->RemoveActor(_current_actor);
    _current_actor = nullptr;

    _vtk_widget->update();
    _vtk_widget->renderWindow()->Render();

  }

}

void MainWindow::slot_btn_cylinder_clicked() {

  std::cout << "slot_btn_perform_clicked()" << std::endl;

  if (_current_actor.Get() == nullptr) {

    std::cout << "The current actor is empty, creating cylinder!" << std::endl;

    using PtrCylinderSource = vtkSmartPointer<vtkCylinderSource>;
    using PtrPolyDataMapper = vtkSmartPointer<vtkPolyDataMapper>;
    using PtrActor = vtkSmartPointer<vtkActor>;

    PtrCylinderSource cylinder_source = PtrCylinderSource::New();
    cylinder_source->SetResolution(300);

    PtrPolyDataMapper poly_data_mapper = PtrPolyDataMapper::New();
    poly_data_mapper->SetInputConnection(cylinder_source->GetOutputPort());

    _current_actor = PtrActor::New();
    _current_actor->SetMapper(poly_data_mapper);
    _current_actor->GetProperty()->SetColor(_actor_red, _actor_green, _actor_blue);
    _current_actor->RotateX(30.0);
    _current_actor->RotateY(-45.0);

    _renderer->AddActor(_current_actor);
    _renderer->ResetCamera();
    _renderer->GetActiveCamera()->Zoom(1.5);

    _vtk_widget->update();
    _vtk_widget->renderWindow()->Render();

  } else {

    std::cout << "There is already active actor!" << std::endl;

  }

}

void MainWindow::slot_btn_cube_clicked() {

  cout << "slot_btn_cube_clicked()" << std::endl;

  if (_current_actor.Get() == nullptr) {

    std::cout << "The current actor is empty, creating cube!" << std::endl;

    using PtrCubeSource = vtkSmartPointer<vtkCubeSource>;
    using PtrPolyDataMapper = vtkSmartPointer<vtkPolyDataMapper>;
    using PtrActor = vtkSmartPointer<vtkActor>;

    PtrCubeSource cube_source = PtrCubeSource::New();

    PtrPolyDataMapper poly_data_mapper = PtrPolyDataMapper::New();
    poly_data_mapper->SetInputConnection(cube_source->GetOutputPort());

    _current_actor = PtrActor::New();
    _current_actor->SetMapper(poly_data_mapper);
    _current_actor->GetProperty()->SetColor(_actor_red, _actor_green, _actor_blue);
    _current_actor->RotateX(30.0);
    _current_actor->RotateY(-45.0);

    _renderer->AddActor(_current_actor);
    _renderer->ResetCamera();
    _renderer->GetActiveCamera()->Zoom(1.5);

    _vtk_widget->update();
    _vtk_widget->renderWindow()->Render();

  } else {

    std::cout << "There is already active actor!" << std::endl;

  }

}
//
// Created by Lesleis Nagy on 23/07/2024.
//

#include <iostream>

#include <QVTKOpenGLNativeWidget.h>
#include <QtCharts/QChart>
#include <QFileDialog>

#include <vtkPolyDataMapper.h>
#include <QErrorMessage>
#include <QMessageBox>
#include <vtkSphereSource.h>
#include "vtkCylinderSource.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkCoordinate.h"
#include "vtkRendererCollection.h"

#include "config_consts.h"
#include "main_window.hpp"
#include "load_tecplot.hpp"

main_window::main_window() {

  // Set up the GUI.
  this->setupUi(this);

  // Retrieve the settings object.
  QSettings settings;

  _txt_img_width->setText(
      settings.value(CONFIG_CURRENT_IMAGE_WIDTH, 100).toString()
  );
  _txt_img_height->setText(
      settings.value(CONFIG_CURRENT_IMAGE_HEIGHT, 100).toString()
  );

  _txt_nx->setText(
      settings.value(CONFIG_CURRENT_NX, 30).toString()
  );
  _txt_ny->setText(
      settings.value(CONFIG_CURRENT_NY, 30).toString()
  );

  // Set up validators
  _txt_nx->setValidator(&_int_validator);
  _txt_ny->setValidator(&_int_validator);
  _txt_img_width->setValidator(&_int_validator);
  _txt_img_height->setValidator(&_int_validator);

  // Set default `_current_image` width/height values.
  int current_image_width = _txt_img_width->text().toInt();
  int current_image_height = _txt_img_height->text().toInt();

  _current_image->set_width(current_image_width);
  _current_image->set_height(current_image_height);

  // Additional GUI widget setup.
  _status_bar->showMessage("Current file: <None>");

  _renderer = _vtk_widget->renderWindow()->GetRenderers()->GetFirstRenderer();
  if (!_renderer) {
    std::cout << "There is no first renderer, so we create one." << std::endl;
    _renderer = vtkSmartPointer<vtkRenderer>::New();
    _renderer->SetBackground(0.0, 0.0, 0.0);
    _renderer->ResetCamera();
    //_renderer->GetActiveCamera()->SetParallelProjection(true);
    _vtk_widget->renderWindow()->AddRenderer(_renderer);
  }

  _interactor = TrackballInteractor::New();
  _interactor->SetDefaultRenderer(_renderer);
  //_vtk_widget->renderWindow()->GetInteractor()->SetInteractorStyle(
  //    _interactor
  //);

  // Set the _current_actor to nullptr.
  _current_actor = nullptr;

  // Connect signals and slots.
  connect(_btn_load_tecplot, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_load_tecplot_clicked()));
  connect(_btn_clear, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_clear_clicked()));
  connect(_btn_mfm, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_mfm_clicked()));
  connect(_btn_holography, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_holography_clicked()));
  connect(_btn_save_image, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_save_image_clicked()));
  connect(_txt_nx, SIGNAL(textChanged(const QString &)),
          this, SLOT(slot_txt_nx_text_changed(const QString &)));

  emit(_current_image->update_image());

}

void main_window::slot_btn_load_tecplot_clicked() {

  QSettings settings;

  QString last_dir = settings.value(CONFIG_LAST_DATA_DIR, "").toString();

  QString selected_file_name = QFileDialog::getOpenFileName(
      this,
      tr("Open Tecplot file"),
      last_dir
  );
  if (selected_file_name.isEmpty()) {
    return;
  }

  QFileInfo file_info{selected_file_name};

  QString abs_path = file_info.absolutePath();
  QString file_name = file_info.fileName();

  settings.setValue(CONFIG_LAST_DATA_DIR, abs_path);

  // Load tecplot file in to _model.
  _model = TecplotFileLoader::read(
      file_info.absoluteFilePath().toStdString()
  );

  _model->enable_graphics();

  _current_actor = _model->u_grid_actor();
  _renderer->AddActor(_current_actor);

  _vtk_widget->update();
  _vtk_widget->renderWindow()->Render();

  _status_bar->showMessage(tr("Current file: ") + file_info.absoluteFilePath());

  _interactor->set_actor(_current_actor);

}

void main_window::slot_btn_clear_clicked() {

  std::cout << "slot_btn_clear_clicked()" << std::endl;


  /*
  if (!_four_corners.empty()) {
    for (const auto& actor : _four_corners) {
      _renderer->RemoveActor(actor);
    }
    _four_corners.erase(_four_corners.begin(), _four_corners.end());
  }

  if (_current_actor.Get() != nullptr) {

    _renderer->RemoveActor(_current_actor);
    _current_actor = nullptr;

    _vtk_widget->update();
    _vtk_widget->renderWindow()->Render();

  }

  _current_image->set_width(Configuration::get_instance().image_x_res);
  _current_image->set_height(Configuration::get_instance().image_y_res);

  emit(_current_image->update_image());
   */

  if (!_four_corners.empty()) {
    for (const auto &actor : _four_corners) {
      _renderer->RemoveActor(actor);
    }
    _four_corners.erase(_four_corners.begin(), _four_corners.end());
  }

  _renderer->ResetCameraClippingRange();


  //_renderer->ResetCamera();

  // _renderer->GetActiveCamera()->GetEyePosition()

  auto width = (double) _vtk_widget->width();
  auto height = (double) _vtk_widget->height();

  std::array<double, 3> bottom_left_corner = {0.0, 0.0, 0.0};
  std::array<double, 3> bottom_right_corner = {2 * width, 0.0, 0.0};
  std::array<double, 3> top_right_corner = {2 * width, 2 * height, 0.0};
  std::array<double, 3> top_left_corner = {0.0, 2 * height, 0.0};

  std::array<double, 3> bottom_left_color = {1.0, 0.0, 0.0};
  std::array<double, 3> bottom_right_color = {0.0, 1.0, 0.0};
  std::array<double, 3> top_right_color = {0.0, 0.0, 1.0};
  std::array<double, 3> top_left_color = {1.0, 1.0, 1.0};

  std::array<std::array<double, 3>, 4> corners{
      bottom_left_corner,
      bottom_right_corner,
      top_right_corner,
      top_left_corner
  };

  std::array<std::array<double, 3>, 4> colors{
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
  std::copy(res, res + 3, wc_tl.begin());
  std::cout << "wc_tl: " << wc_tl[0] << ", " << wc_tl[1] << ", " << wc_tl[2]
            << std::endl;

  coordinate->SetValue(
      top_right_corner[0],
      top_right_corner[1],
      top_right_corner[2]
  );
  res = coordinate->GetComputedWorldValue(_renderer);
  std::copy(res, res + 3, wc_br.begin());
  std::cout << "wc_br: " << wc_br[0] << ", " << wc_br[1] << ", " << wc_br[2]
            << std::endl;

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
    std::copy(res, res + 3, wc.begin());

    std::cout << "World equivalent of ("
              << c[0] << ", " << c[1] << ", " << c[2] << "): "
              << wc[0] << ", " << wc[1] << ", " << wc[2] << ")"
              << std::endl;

    vtkSmartPointer<vtkSphereSource> sphere_src = vtkSphereSource::New();
    sphere_src->SetPhiResolution(50);
    sphere_src->SetThetaResolution(50);
    sphere_src->SetRadius(len * 0.05);

    vtkSmartPointer<vtkPolyDataMapper> sphere_map = vtkPolyDataMapper::New();
    sphere_map->SetInputConnection(sphere_src->GetOutputPort());

    vtkSmartPointer<vtkActor> sphere_act = vtkActor::New();
    sphere_act->SetMapper(sphere_map);
    sphere_act->SetPosition(wc[0], wc[1], wc[2]);
    sphere_act->GetProperty()->SetColor(color[0], color[1], color[2]);

    _four_corners.push_back(sphere_act);

    _renderer->AddActor(sphere_act);

  }

  _renderer->ResetCameraClippingRange();

  _vtk_widget->update();
  _vtk_widget->renderWindow()->Render();

}

void main_window::slot_btn_mfm_clicked() {

  std::cout << "slot_btn_mfm_clicked()" << std::endl;


  /*
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
   */

}

void main_window::slot_btn_holography_clicked() {

  cout << "slot_btn_holography_clicked()" << std::endl;

  /*
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
  */

}

void
main_window::slot_btn_save_image_clicked() {

  cout << "slot_btn_save_image_clicked()" << std::endl;

  // Get save file location from the user.

  QSettings settings;

  QString last_save_dir = settings.value(CONFIG_LAST_IMG_SAVE_DIR, "").toString();

  std::array<QString, 2> exts = {
      tr("png"),
      tr("jpg")
  };

  QString output_file_name = QFileDialog::getSaveFileName(
      this,
      tr("Open Tecplot file"),
      last_save_dir
  );
  if (output_file_name.isEmpty()) {
    return;
  }

  QFileInfo file_info{output_file_name};

  QString abs_path = file_info.absolutePath();
  QString abs_file_path = file_info.absoluteFilePath();
  QString file_name = file_info.fileName();
  QString suffix = file_info.suffix();

  settings.setValue(CONFIG_LAST_IMG_SAVE_DIR, abs_path);

  std::cout << "abs_path:      " << abs_path.toStdString() << std::endl;
  std::cout << "abs_file_path: " << abs_file_path.toStdString() << std::endl;
  std::cout << "file_name:     " << file_name.toStdString() << std::endl;
  std::cout << "file_suffix:   " << suffix.toStdString() << std::endl;

  if (std::find(exts.begin(), exts.end(), suffix) == exts.end()) {
    QMessageBox::warning(
        nullptr,
        "Error",
        tr("The suffix ") + suffix + tr(" is not recognised.")
    );
  }

  emit(_current_image->save_file(abs_file_path));

}

void
main_window::slot_txt_nx_text_changed(const QString &text) {

  std::cout << "slot_txt_nx_text_changed() --> text = '" << text.toStdString() << "'" << std::endl;

}

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

  _current_actor = _model->ugrid_actor();
  _renderer->AddActor(_current_actor);

  _vtk_widget->update();
  _vtk_widget->renderWindow()->Render();

  _status_bar->showMessage(tr("Current file: ") + file_info.absoluteFilePath());

  //_interactor->set_actor(_current_actor);

}

void main_window::slot_btn_clear_clicked() {

  _renderer->ResetCameraClippingRange();

  _vtk_widget->update();
  _vtk_widget->renderWindow()->Render();

}

void main_window::slot_btn_mfm_clicked() {

  std::cout << "slot_btn_mfm_clicked()" << std::endl;

}

void main_window::slot_btn_holography_clicked() {

  cout << "slot_btn_holography_clicked()" << std::endl;

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

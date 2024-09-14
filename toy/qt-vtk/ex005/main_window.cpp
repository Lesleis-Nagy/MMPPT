//
// Created by Lesleis Nagy on 23/07/2024.
//


#include "main_window.hpp"


MainWindow::MainWindow() {

  // Set up the GUI.
  this->setupUi(this);

  // Create the settings menu
  _settingsMenu = new QMenu("Settings", this);
  menuBar()->addMenu(_settingsMenu);

  // Add actions to the settings menu
  _preferencesAction = _settingsMenu->addAction("Preferences");
  _preferencesAction->setMenuRole(QAction::PreferencesRole);

  _aboutAction = _settingsMenu->addAction("About");
  _aboutAction->setMenuRole(QAction::AboutRole);

  // Retrieve the settings object.
  QSettings settings;

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

  // Connect signals and slots.
  connect(_btn_load_tecplot, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_load_tecplot_clicked()));
  connect(_btn_clear, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_clear_clicked()));
  connect(_btn_set_arrow_scale, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_set_arrow_scale_clicked()));
  connect(_btn_camera_x, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_camera_x_clicked()));
  connect(_btn_camera_y, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_camera_y_clicked()));
  connect(_btn_camera_z, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_camera_z_clicked()));
  connect(_btn_put_plane, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_put_plane_clicked()));
  connect(_btn_remove_plane, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_remove_plane_clicked()));

  connect(_chk_ugrid, SIGNAL(checkStateChanged(Qt::CheckState)),
          this, SLOT(slot_chk_ugrid_changed(Qt::CheckState)));
  connect(_chk_vectors, SIGNAL(checkStateChanged(Qt::CheckState)),
          this, SLOT(slot_chk_vectors_changed(Qt::CheckState)));
  connect(_sli_ugrid_opacity, SIGNAL(valueChanged(int)),
          this, SLOT(slot_sli_ugrid_opacity_changed(int)));
  connect(_sli_vector_opacity, SIGNAL(valueChanged(int)),
          this, SLOT(slot_sli_vector_opacity_changed(int)));

  connect(_btn_mfm, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_mfm_clicked()));
  connect(_btn_holography, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_holography_clicked()));
  connect(_btn_save_image, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_save_image_clicked()));

  connect(_preferencesAction, &QAction::triggered,
          this, &MainWindow::slot_menu_preferences);

  emit(_current_image->update_image());

}

void
MainWindow::slot_timer_timeout() {

}


void
MainWindow::slot_btn_load_tecplot_clicked() {

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

  clear_model();

  QFileInfo file_info{selected_file_name};

  QString abs_path = file_info.absolutePath();
  QString file_name = file_info.fileName();

  settings.setValue(CONFIG_LAST_DATA_DIR, abs_path);

  // Load tecplot file in to _model.
  _model = TecplotFileLoader::read(
      file_info.absoluteFilePath().toStdString()
  );

  _model->enable_graphics();
  _model->set_arrow_scale(_txt_arrow_scale->text().toDouble());

  show_arrow_actor();
  show_ugrid_actor();

  _chk_ugrid->setCheckState(Qt::CheckState::Checked);
  _chk_vectors->setCheckState(Qt::CheckState::Checked);

  _status_bar->showMessage(tr("Current file: ") + file_info.absoluteFilePath());

  _vtk_widget->update();
  _vtk_widget->renderWindow()->Render();

}

void
MainWindow::slot_btn_clear_clicked() {

  clear_model();

  _renderer->ResetCameraClippingRange();

  _vtk_widget->update();
  _vtk_widget->renderWindow()->Render();

}

void
MainWindow::slot_btn_set_arrow_scale_clicked() {

  set_arrow_scale(_txt_arrow_scale->text().toDouble());

  _vtk_widget->update();
  _vtk_widget->renderWindow()->Render();

}

void
MainWindow::slot_btn_camera_x_clicked() {

  set_camera_to_x_pos();

  _renderer->ResetCamera();
  _renderer->ResetCameraClippingRange();
  _vtk_widget->update();
  _vtk_widget->renderWindow()->Render();

}

void
MainWindow::slot_btn_camera_y_clicked() {

  set_camera_to_y_pos();

  _renderer->ResetCamera();
  _renderer->ResetCameraClippingRange();
  _vtk_widget->update();
  _vtk_widget->renderWindow()->Render();

}

void
MainWindow::slot_btn_camera_z_clicked() {

  set_camera_to_z_pos();

  _renderer->ResetCamera();
  _renderer->ResetCameraClippingRange();
  _vtk_widget->update();
  _vtk_widget->renderWindow()->Render();

}

void
MainWindow::slot_btn_put_plane_clicked() {

  std::cout << "slot_btn_put_plane_clicked" << std::endl;

  // TODO: Check to see if I can put a plane, i.e. if there is any data in
  //       the boxes.

  // Set the plane _plane_actor.
  _plane_source = vtkPlaneSource::New();
  _plane_source->SetResolution(10, 10);
  _plane_source->SetCenter(0, 0, 0);
  _plane_source->SetNormal(0, 0, 1);
  _plane_source->Update();

  _plane_poly_data_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  _plane_poly_data_mapper->SetInputConnection(_plane_source->GetOutputPort());

  _plane_actor = vtkActor::New();
  _plane_actor->SetMapper(_plane_poly_data_mapper);
  _plane_actor->GetProperty()->SetColor(1, 1, 1);
  _plane_actor->GetProperty()->LightingOff();

  _renderer->AddActor(_plane_actor);


  _vtk_widget->update();
  _vtk_widget->renderWindow()->Render();

}

void
MainWindow::slot_btn_remove_plane_clicked() {

  std::cout << "slot_btn_remove_plane_clicked" << std::endl;

}

void
MainWindow::slot_chk_ugrid_changed(Qt::CheckState state) {

  if (state == Qt::CheckState::Checked) {
    show_ugrid_actor();
  } else {
    hide_ugrid_actor();
  }

  _vtk_widget->update();
  _vtk_widget->renderWindow()->Render();

}

void
MainWindow::slot_chk_vectors_changed(Qt::CheckState state) {

  if (state == Qt::CheckState::Checked) {
    show_arrow_actor();
  } else {
    hide_arrow_actor();
  }

  _vtk_widget->update();
  _vtk_widget->renderWindow()->Render();

}

void
MainWindow::slot_sli_ugrid_opacity_changed(int value) {

  set_ugrid_opacity((double) value / 1000.0);

  _vtk_widget->update();
  _vtk_widget->renderWindow()->Render();

}

void
MainWindow::slot_sli_vector_opacity_changed(int value) {

  set_arrow_opacity((double) value / 1000.0);

  _vtk_widget->update();
  _vtk_widget->renderWindow()->Render();

}

void MainWindow::slot_btn_mfm_clicked() {

  std::cout << "slot_btn_mfm_clicked()" << std::endl;

}

void MainWindow::slot_btn_holography_clicked() {

  cout << "slot_btn_holography_clicked()" << std::endl;

}

void
MainWindow::slot_btn_save_image_clicked() {

  cout << "slot_btn_save_image_clicked()" << std::endl;

  // Get save file location from the user.

  QSettings settings;

  QString
      last_save_dir = settings.value(CONFIG_LAST_IMG_SAVE_DIR, "").toString();

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
MainWindow::slot_menu_preferences() {

  std::cout << "slot_menu_preferences()" << std::endl;

  PreferencesDialog preferences_dialog;
  preferences_dialog.exec();

}

//----------------------------------------------------------------------------

void
MainWindow::clear_model() {

  hide_ugrid_actor();
  hide_arrow_actor();

  _chk_ugrid->setCheckState(Qt::CheckState::Unchecked);
  _chk_vectors->setCheckState(Qt::CheckState::Unchecked);

}

void
MainWindow::hide_ugrid_actor() {

  if (_model.has_value()) {
    if (_model_ugrid_actor_showing) {
      _model->remove_ugrid_actor(_renderer);
      _model_ugrid_actor_showing = false;
    }
  }

}

void
MainWindow::show_ugrid_actor() {

  if (_model.has_value()) {
    if (!_model_ugrid_actor_showing) {
      _model->add_ugrid_actor(_renderer);
      _model_ugrid_actor_showing = true;
    }
  }
}

void
MainWindow::hide_arrow_actor() {
  if (_model.has_value()) {
    if (_model_arrow_actor_showing) {
      _model->remove_arrow_actor(_renderer);
      _model_arrow_actor_showing = false;
    }
  }

}

void
MainWindow::show_arrow_actor() {
  if (_model.has_value()) {
    if (!_model_arrow_actor_showing) {
      _model->add_arrow_actor(_renderer);
      _model_arrow_actor_showing = true;
    }
  }
}

void
MainWindow::set_ugrid_opacity(double opacity) {

  if (_model.has_value()) {
    _model->set_ugrid_opacity(opacity);
  }

}

void
MainWindow::set_arrow_opacity(double opacity) {

  if (_model.has_value()) {
    _model->set_arrow_opacity(opacity);
  }

}

void
MainWindow::set_arrow_scale(double scale) {

  if (_model.has_value()) {
    _model->set_arrow_scale(scale);
  }

}

void
MainWindow::set_camera_to_x_pos() {

  if (_model.has_value()) {
    std::array c = _model->center();
    _renderer->GetActiveCamera()->SetPosition(-1, 0, 0);
    _renderer->GetActiveCamera()->SetFocalPoint(c[0], c[1], c[2]);
    _renderer->GetActiveCamera()->SetViewUp(0, 1, 0);
  }

}

void
MainWindow::set_camera_to_y_pos() {

  if (_model.has_value()) {
    std::array c = _model->center();
    _renderer->GetActiveCamera()->SetPosition(0, -1, 0);
    _renderer->GetActiveCamera()->SetFocalPoint(c[0], c[1], c[2]);
    _renderer->GetActiveCamera()->SetViewUp(1, 0, 0);
  }

}

void
MainWindow::set_camera_to_z_pos() {

  if (_model.has_value()) {
    std::array c = _model->center();
    _renderer->GetActiveCamera()->SetPosition(0, 0, -1);
    _renderer->GetActiveCamera()->SetFocalPoint(c[0], c[1], c[2]);
    _renderer->GetActiveCamera()->SetViewUp(0, 1, 0);
  }

}

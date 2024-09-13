//
// Created by Lesleis Nagy on 13/09/2024.
//

#include "preferences_dialog.hpp"

PreferencesDialog::PreferencesDialog() {
  this->setupUi(this);

  _txt_nx->setText(_settings.value(CONFIG_CURRENT_NX, "30").toString());
  _txt_ny->setText(_settings.value(CONFIG_CURRENT_NY, "30").toString());

  _txt_image_width->setText(
      _settings.value(CONFIG_CURRENT_IMAGE_WIDTH, "100").toString()
  );
  _txt_image_height->setText(
      _settings.value(CONFIG_CURRENT_IMAGE_HEIGHT, "100").toString()
  );

  connect(_btn_close, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_close_clicked()));
}

PreferencesDialog::~PreferencesDialog() {

  std::cout << "Preferences dialog destructor." << std::endl;

  _settings.setValue(CONFIG_CURRENT_NX, _txt_nx->text());
  _settings.setValue(CONFIG_CURRENT_NY, _txt_ny->text());
  _settings.setValue(CONFIG_CURRENT_IMAGE_WIDTH, _txt_image_width->text());
  _settings.setValue(CONFIG_CURRENT_IMAGE_HEIGHT, _txt_image_height->text());

}

void PreferencesDialog::slot_btn_close_clicked() {

  std::cout << "Close clicked" << std::endl;

  emit(this->close());

}
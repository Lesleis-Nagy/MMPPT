//
// Created by Lesleis Nagy on 13/09/2024.
//

#ifndef MMPPT_TOY_QT_VTK_EX005_PREFERENCES_DIALOG_HPP_
#define MMPPT_TOY_QT_VTK_EX005_PREFERENCES_DIALOG_HPP_

#include "ui_preferences_dialog.h"

#include <iostream>

#include <QDialog>
#include <QSettings>

#include "config_consts.h"

 class PreferencesDialog : public QDialog, private Ui::PreferencesDialog {

  Q_OBJECT

 public:

  PreferencesDialog();
  ~PreferencesDialog() override;

 public slots:

  void slot_btn_close_clicked();

 private:

  QSettings _settings;

};

#endif //MMPPT_TOY_QT_VTK_EX005_PREFERENCES_DIALOG_HPP_

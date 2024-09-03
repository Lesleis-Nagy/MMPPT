//
// Created by Lesleis Nagy on 03/09/2024.
//

#ifndef MMPPT_TOY_QT_VTK_EX005_CONFIG_HPP_
#define MMPPT_TOY_QT_VTK_EX005_CONFIG_HPP_

#include <QString>
#include <QSettings>

class Configuration {

 public:

  static Configuration& get_instance() {
    static Configuration instance;
    return instance;
  }

  void load_settings() {
    QSettings settings("liv.ac.uk", "MMFFP");
    image_interpolation = settings.value("image_interpolation", "none").toString();
    image_x_res = settings.value("image_x_res", 200).toInt();
    image_y_res = settings.value("image_y_res", 200).toInt();
  }

  void save_settings() {
    QSettings settings("liv.ac.uk", "MMFFP");
    settings.setValue("image_interpolation", image_interpolation);
    settings.setValue("image_x_res", image_x_res);
    settings.setValue("image_y_res", image_y_res);

  }

  QString image_interpolation;
  int image_x_res;
  int image_y_res;

 private:

  // Private constructor, ensure singleton
  Configuration() {}
  Configuration(const Configuration&) = delete;
  Configuration & operator=(const Configuration&) = delete;

};

#endif // MMPPT_TOY_QT_VTK_EX005_CONFIG_HPP_

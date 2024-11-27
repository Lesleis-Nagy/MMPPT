//
// Created by Lesleis Nagy on 02/09/2024.
//
#include <iostream>
#include "pixel_widget.h"

#include "color.hpp"
#include "palettes.hpp"
#include "map.hpp"

PixelWidget::PixelWidget(QWidget *parent)
    : _nx(10), _ny(10), QWidget(parent) {

  generate_blank_image();

}

void
PixelWidget::paintEvent(QPaintEvent *) {

  QPainter painter(this);
  painter.setRenderHint(QPainter::LosslessImageRendering);
  painter.drawImage(0, 0, _image.scaled(size(), Qt::IgnoreAspectRatio));

}

void
PixelWidget::blank_image() {

  generate_blank_image();

}

void
PixelWidget::update_image() {

  generate_random_image();

}

void
PixelWidget::update_image(const std::vector<std::vector<double>> &scalars) {

  _image = QImage(_nx, _ny, QImage::Format_RGB32);

  double scalar_min =  1E99;
  double scalar_max = -1E99;
  for (const auto & row : scalars) {
    for (double value : row) {
      if (value < scalar_min) scalar_min = value;
      if (value > scalar_max) scalar_max = value;
    }
  }

  std::cout << "scalar min: " << scalar_min << std::endl;
  std::cout << "scalar max: " << scalar_max << std::endl;

  auto palette = colormap::palettes.at("greys").rescale(scalar_min, scalar_max);

  for (int i = 0; i < _image.width(); ++i) {
    for (int j = 0; j < _image.height(); ++j) {
      auto colour = palette(scalars[i][j]);
      _image.setPixel(i, j, qRgb(
          colour.getRed().getValue(),
          colour.getGreen().getValue(),
          colour.getBlue().getValue())
      );
    }
  }

  this->update();

}

int
PixelWidget::nx() const {
  return _nx;
}

int
PixelWidget::ny() const {
  return _ny;
}

void PixelWidget::nx(int value) {

  _nx = value;
  _image = QImage(_nx, _ny, QImage::Format_RGB32);

}

void PixelWidget::ny(int value) {

  _ny = value;
  _image = QImage(_nx, _ny, QImage::Format_RGB32);

}

void PixelWidget::save_file(const QString &destination) {

  _image.save(destination);

}

void
PixelWidget::generate_blank_image() {

  _image = QImage(_nx, _ny, QImage::Format_RGB32);

  const auto &palette = colormap::palettes.at("greys");

  for (int i = 0; i < _image.width(); ++i) {
    for (int j = 0; j < _image.height(); ++j) {
      _image.setPixel(i, j, qRgb(0.0, 0.0, 0.0));
    }
  }

  this->update();

}

void
PixelWidget::generate_random_image() {

  _image = QImage(_nx, _ny, QImage::Format_RGB32);

  const auto &palette = colormap::palettes.at("greys");

  for (int i = 0; i < _image.width(); ++i) {
    for (int j = 0; j < _image.height(); ++j) {
      double rand = QRandomGenerator64::global()->generateDouble();
      auto colour = palette(rand);
        _image.setPixel(i, j, qRgb(
            colour.getRed().getValue(),
            colour.getGreen().getValue(),
            colour.getBlue().getValue())
        );
    }
  }

  this->update();

}
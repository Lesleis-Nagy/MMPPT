//
// Created by Lesleis Nagy on 02/09/2024.
//
#include <iostream>
#include "pixel_widget.h"

#include "color.hpp"
#include "palettes.hpp"
#include "map.hpp"

PixelWidget::PixelWidget(QWidget *parent)
    : _width(10), _height(10), QWidget(parent) {

  // Generate a random image
  generate_random_image();

}

void
PixelWidget::paintEvent(QPaintEvent *) {

  QPainter painter(this);
  painter.setRenderHint(QPainter::LosslessImageRendering);
  painter.drawImage(0, 0, _image.scaled(size(), Qt::IgnoreAspectRatio));

}

void PixelWidget::update_image() {

  generate_random_image();

}

void PixelWidget::set_width(int width) {

  _width = width;
  _image = QImage(_width, _height, QImage::Format_RGB32);

}

void PixelWidget::set_height(int height) {

  _height = height;
  _image = QImage(_width, _height, QImage::Format_RGB32);

}

void PixelWidget::save_file(const QString &destination) {

  _image.save(destination);

}

void
PixelWidget::generate_random_image() {

  _image = QImage(_width, _height, QImage::Format_RGB32);

  const auto &palette = colormap::palettes.at("blues");

  for (int i = 0; i < _image.width(); ++i) {
    for (int j = 0; j < _image.height(); ++j) {
      double rand = QRandomGenerator64::global()->generateDouble();
      auto colour = palette(rand);

      if ((i==99) && (j==0)) {
        _image.setPixel(i, j, qRgb(
            colour.getRed().getValue(),
            colour.getGreen().getValue(),
            colour.getBlue().getValue())
        );
      } else {
        _image.setPixel(i, j, qRgb(0, 0, 0));
      }

      /*
      _image.setPixel(i, j, qRgb(QRandomGenerator::global()->generate() % 256,
                                 QRandomGenerator::global()->generate() % 256,
                                 QRandomGenerator::global()->generate() % 256));
      */

    }
  }

  this->update();

}
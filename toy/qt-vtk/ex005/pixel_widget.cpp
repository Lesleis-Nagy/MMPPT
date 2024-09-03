//
// Created by Lesleis Nagy on 02/09/2024.
//

#include "pixel_widget.h"

PixelWidget::PixelWidget(QWidget* parent) : _width(10), _height(10), QWidget(parent) {

  // Generate a random image
  generate_random_image();

}

void
PixelWidget::paintEvent(QPaintEvent*) {

  QPainter painter(this);
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

void
PixelWidget::generate_random_image() {

  _image = QImage(_width, _height, QImage::Format_RGB32);

  for (int x = 0; x < _image.width(); ++x) {
    for (int y = 0; y < _image.height(); ++y) {
      _image.setPixel(x, y, qRgb(QRandomGenerator::global()->generate() % 256,
                                 QRandomGenerator::global()->generate() % 256,
                                 QRandomGenerator::global()->generate() % 256));
    }
  }

  this->update();

}


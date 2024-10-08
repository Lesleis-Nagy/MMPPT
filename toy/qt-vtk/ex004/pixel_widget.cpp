//
// Created by Lesleis Nagy on 02/09/2024.
//

#include "pixel_widget.h"

PixelWidget::PixelWidget(QWidget* parent) : QWidget(parent) {
  // Generate a random image
  generate_random_image();
}

void
PixelWidget::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  painter.drawImage(0, 0, _image.scaled(size(), Qt::IgnoreAspectRatio));
  //painter.setRenderHint(QPainter::Antialiasing, false);
}

void
PixelWidget::generate_random_image() {
  const int width = 10;
  const int height = 10;
  _image = QImage(width, height, QImage::Format_RGB32);


  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      _image.setPixel(x, y, qRgb(QRandomGenerator::global()->generate() % 256,
                                 QRandomGenerator::global()->generate() % 256,
                                 QRandomGenerator::global()->generate() % 256));
    }
  }
}


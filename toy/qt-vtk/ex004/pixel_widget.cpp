//
// Created by Lesleis Nagy on 02/09/2024.
//

#include "pixel_widget.h"

PixelWidget::PixelWidget(QWidget* parent) : QWidget(parent) {
  // Generate a random image
  generateRandomImage();
}

void
PixelWidget::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  painter.drawImage(0, 0, m_image.scaled(size(), Qt::IgnoreAspectRatio));
  //painter.setRenderHint(QPainter::Antialiasing, false);
}

void
PixelWidget::generateRandomImage() {
  const int width = 10;
  const int height = 10;
  m_image = QImage(width, height, QImage::Format_RGB32);


  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      m_image.setPixel(x, y, qRgb(QRandomGenerator::global()->generate() % 256,
                                  QRandomGenerator::global()->generate() % 256,
                                  QRandomGenerator::global()->generate() % 256));
    }
  }
}


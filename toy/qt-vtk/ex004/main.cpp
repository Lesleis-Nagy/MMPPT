//
// Created by Lesleis Nagy on 23/07/2024.
//

#include <QApplication>
#include <QVTKOpenGLWindow.h>

#include "MainWindow.hpp"

int main(int argc, char *argv[]) {

  QSurfaceFormat::setDefaultFormat(QVTKOpenGLWindow::defaultFormat());

  QApplication app(argc, argv);

  main_window main_window;
  main_window.show();

  return QApplication::exec();

}

/*
#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QWidget>

class PixelDrawWidget : public QWidget {
 public:
  explicit PixelDrawWidget(QWidget* parent = nullptr) : QWidget(parent) {
    // Set the initial image size
    m_image = QImage(10, 10, QImage::Format_ARGB32);
    m_image.fill(Qt::white);
  }

 protected:
  void paintEvent(QPaintEvent*) override {
    QPainter painter(this);

    // Draw the image pixel-by-pixel
    for (int x = 0; x < m_image.width(); x++) {
      for (int y = 0; y < m_image.height(); y++) {
        // Set the pixel color
        m_image.setPixel(x, y, qRgb(rand() % 256, rand() % 256, rand() % 256));
      }
    }

    // Draw the image on the widget
    painter.drawImage(0, 0, m_image);
  }

 private:
  QImage m_image;
};

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  PixelDrawWidget widget;
  widget.show();

  return app.exec();
}
*/


/*

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  PixelWidget widget;
  widget.show();
  return app.exec();
}

*/
//
// Created by Lesleis Nagy on 02/09/2024.
//

#ifndef MMPPT_TOY_QT_VTK_EX005_MAINWINDOW_H_
#define MMPPT_TOY_QT_VTK_EX005_MAINWINDOW_H_

#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QRandomGenerator>
#include <QWidget>
#include <QColor>
#include <QColorTransform>

class PixelWidget : public QWidget {
 Q_OBJECT

 public:

  /**
   * Constructor.
   */
  explicit PixelWidget(QWidget *parent = nullptr);

 public slots:

  void update_image();
  void set_width(int width);
  void set_height(int height);
  void save_file(const QString& destination);

 protected:

  void paintEvent(QPaintEvent *) override;

 private:

  void generate_random_image();

  int _width;
  int _height;

  QImage _image;

};

#endif // MMPPT_TOY_QT_VTK_EX005_MAINWINDOW_H_

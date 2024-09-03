//
// Created by Lesleis Nagy on 02/09/2024.
//

#ifndef MICROMAGPOSTPROCESSINGTOOLS_TOY_QT_VTK_EX004_PIXEL_WIDGET_H_
#define MICROMAGPOSTPROCESSINGTOOLS_TOY_QT_VTK_EX004_PIXEL_WIDGET_H_

#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QRandomGenerator>
#include <QWidget>

class PixelWidget : public QWidget {
  Q_OBJECT

 public:
  explicit PixelWidget(QWidget* parent=nullptr);

 protected:
  void paintEvent(QPaintEvent*) override;

 private:
  void generate_random_image();

  QImage _image;
};


#endif //MICROMAGPOSTPROCESSINGTOOLS_TOY_QT_VTK_EX004_PIXEL_WIDGET_H_

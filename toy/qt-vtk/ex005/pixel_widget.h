//
// Created by Lesleis Nagy on 02/09/2024.
//

#ifndef MMPPT_TOY_QT_VTK_EX005_MAINWINDOW_H_
#define MMPPT_TOY_QT_VTK_EX005_MAINWINDOW_H_

#include <vector>

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
  void blank_image();
  void update_image();
  void update_image(const std::vector<std::vector<double>> &);

  [[nodiscard]] int nx() const;
  [[nodiscard]] int ny() const;

 public slots:

  void nx(int value);
  void ny(int value);
  void save_file(const QString &destination);

 protected:

  void paintEvent(QPaintEvent *) override;

 private:

  void generate_blank_image();
  void generate_random_image();

  int _nx;
  int _ny;

  QImage _image;

};

#endif // MMPPT_TOY_QT_VTK_EX005_MAINWINDOW_H_

//
// Created by Lesleis Nagy on 23/07/2024.
//

#include <iostream>

#include <QVTKOpenGLNativeWidget.h>
#include <QRegularExpression>

#include "MainWindow.hpp"

MainWindow::MainWindow() {

  this->setupUi(this);

  // Set validation.
  QRegularExpression regex_real(R"([-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?)");
  QRegularExpression regex_int(R"([0-9]+)");

  _real_validator = QPointer(new QRegularExpressionValidator(regex_real));
  _int_validator = QPointer(new QRegularExpressionValidator(regex_int));

  _txt_x_min->setValidator(_real_validator);
  _txt_x_max->setValidator(_real_validator);

  _txt_x_min->setValidator(_real_validator);
  _txt_y_max->setValidator(_real_validator);

  _txt_nx->setValidator(_int_validator);
  _txt_ny->setValidator(_int_validator);

  // this will also allow rescaling the color scale by dragging/zooming
  _plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  _plot->axisRect()->setupFullAxesBox(true);
  _plot->xAxis->setLabel("x values");
  _plot->yAxis->setLabel("y values");

  // set up the QCPColorMap
  _color_map = QPointer(new QCPColorMap(_plot->xAxis, _plot->yAxis));

  // add a color scale:
  _color_scale = QPointer(new QCPColorScale(_plot));

  // make sure the axis rect and color scale synchronize their bottom and top
  // margins (so they line up):
  _margin_group = QPointer(new QCPMarginGroup(_plot));

  // Connect signals and slots.
  connect(_btn_draw, SIGNAL(clicked(bool)),
          this, SLOT(slot_btn_draw_clicked()));

  draw_plot();

}

MainWindow::~MainWindow() = default;

void MainWindow::slot_btn_draw_clicked() {

  std::cout << "slot_btn_draw_clicked()" << std::endl;

  draw_plot();

}

void
MainWindow::draw_plot() {

  int nx = _txt_nx->text().toInt();
  int ny = _txt_ny->text().toInt();

  // we want the color map to have nx * ny data points
  _color_map->data()->setSize(nx, ny);

  // and span the coordinate range -4..4 in both key (x) and value (y)
  // dimensions
  _color_map->data()->setRange(
      QCPRange(_txt_x_min->text().toDouble(), _txt_x_max->text().toDouble()),
      QCPRange(_txt_y_min->text().toDouble(), _txt_y_max->text().toDouble())
  );

  // now we assign some data, by accessing the QCPColorMapData instance of the
  // color map
  double x, y, z;
  for (int xIndex = 0; xIndex < nx; ++xIndex) {
    for (int yIndex = 0; yIndex < ny; ++yIndex) {
      _color_map->data()->cellToCoord(xIndex, yIndex, &x, &y);

      double r = 3 * qSqrt(x * x + y * y) + 1e-2;

      // the B field strength of dipole radiation (modulo physical constants)
      z = 2 * x * (qCos(r + 2) / r - qSin(r + 2) / r);

      _color_map->data()->setCell(xIndex, yIndex, z);
    }
  }

  // add a color scale:
  //QCPColorScale *colorScale = new QCPColorScale(_plot);
  if (!_plot->plotLayout()->hasElement(0, 1)) {
    // add it to the right of the main axis rect
    _plot->plotLayout()->addElement(0, 1, _color_scale);
  }

  // scale shall be vertical bar with tick/axis labels right (actually atRight
  // is already the default)
  _color_scale->setType(QCPAxis::atRight);

  // associate the color map with the color scale
  _color_map->setColorScale(_color_scale);
  _color_scale->axis()->setLabel("Magnetic Field Strength");

  // set the color gradient of the color map to one of the presets:
  _color_map->setGradient(QCPColorGradient::gpGrayscale);

  // we could have also created a QCPColorGradient instance and added own
  // colors to the gradient, see the documentation of QCPColorGradient for
  // what's possible.

  // rescale the data dimension (color) such that all data points lie in the
  // span visualized by the color gradient:
  _color_map->rescaleDataRange();

  // make sure the axis rect and color scale synchronize their bottom and top
  // margins (so they line up)
  //
  // QCPMarginGroup *marginGroup = new QCPMarginGroup(_plot);
  _plot->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, _margin_group);
  _color_scale->setMarginGroup(QCP::msBottom | QCP::msTop, _margin_group);

  // rescale the key (x) and value (y) axes so the whole color map is visible:
  _plot->rescaleAxes();
  _plot->replot();



}

//
// Created by Lesleis Nagy on 23/10/2024.
//

#ifndef MMPPT_TOY_QT_VTK_EX005_MFM_COMPUTER_HPP_
#define MMPPT_TOY_QT_VTK_EX005_MFM_COMPUTER_HPP_

#include "model.hpp"
#include "sample_plane.hpp"
#include "pixel_widget.h"

#include "fabbri.hpp"

#include <chrono>

/* Only needed for the sake of this example. */
//#include <iostream>
//#include <thread>

class MFMComputer {

 public:

  MFMComputer(const Model &model,
              size_t model_field_index);

  [[nodiscard]] double
  operator() (double x,  double y,  double z,
              double nx, double ny, double nz) const;

  [[nodiscard]] double
  operator() (const lcgl::Vector3D<double> &r,
              const lcgl::Vector3D<double> &n) const;

 private:

  std::vector<PosToVectorMap<double>> _vp_func;

  // Plane normal vector.
  lcgl::Vector3D<double> _n;

};

#endif //MMPPT_TOY_QT_VTK_EX005_MFM_COMPUTER_HPP_

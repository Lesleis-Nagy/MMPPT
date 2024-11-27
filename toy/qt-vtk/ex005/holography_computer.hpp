//
// Created by Lesleis Nagy on 04/11/2024.
//

#ifndef MMPPT_TOY_QT_VTK_EX005_HOLOGRAPHY_COMPUTER_HPP_
#define MMPPT_TOY_QT_VTK_EX005_HOLOGRAPHY_COMPUTER_HPP_

#include "model.hpp"
#include "sample_plane.hpp"
#include "pixel_widget.h"

#include "fabbri.hpp"

#include <chrono>

class HolographyComputer {

 public:

  HolographyComputer(const Model &model,
                     size_t model_field_index,
                     size_t npts=20,
                     double delta=1E-6);

  [[nodiscard]] lcgl::Vector3D<double>
  vector_potential(const lcgl::Vector3D<double> &r) const;

  [[nodiscard]] lcgl::Vector3D<double>
  transformed_vector_potential(
      const lcgl::Vector3D<double> &r0,
      const lcgl::Vector3D<double> &n,
      double t) const;

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

  size_t _npts{20};

  double _delta;

  std::function<lcgl::Vector3D<double>(double)> _transform;


};

#endif //MMPPT_TOY_QT_VTK_EX005_HOLOGRAPHY_COMPUTER_HPP_

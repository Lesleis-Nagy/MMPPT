//
// Created by Lesleis Nagy on 08/09/2024.
//

#ifndef MMPPT_TOY_QT_VTK_EX005_TETRAHEDRON_HPP_
#define MMPPT_TOY_QT_VTK_EX005_TETRAHEDRON_HPP_

#include <array>

namespace tetrahedron {

inline std::array<double, 3>
centroid(const std::array<double, 3> &x0,
         const std::array<double, 3> &x1,
         const std::array<double, 3> &x2,
         const std::array<double, 3> &x3
) {

  return {
      (x0[0] + x1[0] + x2[0] + x3[0]) / 4.0,
      (x0[1] + x1[1] + x2[1] + x3[1]) / 4.0,
      (x0[2] + x1[2] + x2[2] + x3[2]) / 4.0
  };

}

inline double
volume(const std::array<double, 3> &x0,
       const std::array<double, 3> &x1,
       const std::array<double, 3> &x2,
       const std::array<double, 3> &x3
) {

  return (
      - x0[2] * x1[1] * x2[0] + x0[1] * x1[2] * x2[0]
      + x0[2] * x1[0] * x2[1] - x0[0] * x1[2] * x2[1]
      - x0[1] * x1[0] * x2[2] + x0[0] * x1[1] * x2[2]
      + x0[2] * x1[1] * x3[0] - x0[1] * x1[2] * x3[0]
      - x0[2] * x2[1] * x3[0] + x1[2] * x2[1] * x3[0]
      + x0[1] * x2[2] * x3[0] - x1[1] * x2[2] * x3[0]
      - x0[2] * x1[0] * x3[1] + x0[0] * x1[2] * x3[1]
      + x0[2] * x2[0] * x3[1] - x1[2] * x2[0] * x3[1]
      - x0[0] * x2[2] * x3[1] + x1[0] * x2[2] * x3[1]
      + x0[1] * x1[0] * x3[2] - x0[0] * x1[1] * x3[2]
      - x0[1] * x2[0] * x3[2] + x1[1] * x2[0] * x3[2]
      + x0[0] * x2[1] * x3[2] - x1[0] * x2[1] * x3[2]
  ) / 6.0;

}

inline void
add_to(
    std::array<double, 3> &output,
    double weight,
    const std::array<double, 3> &input
) {

  output[0] += weight * input[0];
  output[1] += weight * input[1];
  output[2] += weight * input[2];

}

} // namespace tetrahedron

#endif //MMPPT_TOY_QT_VTK_EX005_TETRAHEDRON_HPP_

//
// Created by Lesleis Nagy on 05/04/2024.
//

#ifndef MMPPT_TOY_QT_VTK_EX005_MESH_HPP_
#define MMPPT_TOY_QT_VTK_EX005_MESH_HPP_

#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <stdexcept>
#include <unordered_map>

#include "aliases.hpp"
#include "tetrahedron.hpp"

//###########################################################################//
//# Mesh.                                                                   #//
//###########################################################################//

/**
 * An object that holds information about a mesh.
 */
class Mesh {

 public:

  /**
   * Constructor will create a new mesh.
   * @param vcl the (v)ertex (c)oordinate (l)ist.
   * @param til the (t)etrahedra (i)ndex (l)ist.
   * @param sml the (s)ub-(m)esh list.
   */
  Mesh(v_list vcl, tet_list til, sm_list sml) :
      _vcl(std::move(vcl)),
      _til(std::move(til)),
      _sml(std::move(sml)),
      _centroid{0.0, 0.0, 0.0},
      _box_max{-DBL_MAX, -DBL_MAX, -DBL_MAX},
      _box_min{DBL_MAX, DBL_MAX, DBL_MAX},
      _volume{0.0} {

    update_mesh_parameters();

  }

  /**
   * Retrieve the vertex coordinate list.
   * @return the vertex coordinate list.
   */
  [[nodiscard]] const v_list &
  vcl() const {

    return _vcl;

  }

  /**
   * Retrieve the tetrahedra index list.
   * @return the tetrahedra index list.
   */
  [[nodiscard]] const tet_list &
  til() const {

    return _til;

  }

  /**
   * Retrieve the sub-mesh index list.
   * @return the sub-mesh index list.
   */
  [[nodiscard]] const sm_list &
  sml() const {

    return _sml;

  }

  /**
   * Retrieve the mesh centroid.
   */
  [[nodiscard]] const std::array<double, 3> &
  centroid() const {

    return _centroid;

  }

  /**
   * Retrieve the box minimum coordinate.
   */
  [[nodiscard]] const std::array<double, 3> &
  box_min() const {

    return _box_min;

  }

  /**
   * Retrieve the box maximum coordinate.
   */
  [[nodiscard]] const std::array<double, 3> &
  box_max() const {

    return _box_max;

  }

  /**
   * Retrieve the mesh volume.
   */
  [[nodiscard]] double
  volume() const {

    return _volume;

  }

 private:

  // Vertex list.
  v_list _vcl;

  // Tetrahedra (index) list.
  tet_list _til;

  // Sub-mesh (index) list.
  sm_list _sml;

  // Some useful mesh parameters.

  std::array<double, 3> _centroid;

  std::array<double, 3> _box_min;

  std::array<double, 3> _box_max;

  double _volume;

  void
  update_mesh_parameters() {

    // Update mesh parameters by vertex.

    for (auto &v : _vcl) {
      update_box_min(v);
      update_box_max(v);
    }

    // Update mesh parameters by element.

    double total_volume = 0.0;
    std::array<double, 3> centroid = {0.0, 0.0, 0.0};

    for (auto &c : _til) {

      auto &x0 = _vcl[c[0]];
      auto &x1 = _vcl[c[1]];
      auto &x2 = _vcl[c[2]];
      auto &x3 = _vcl[c[3]];

      double tet_volume = tetrahedron::volume(x0, x1, x2, x3);
      auto tet_centroid = tetrahedron::centroid(x0, x1, x2, x3);

      total_volume += tet_volume;

      tetrahedron::add_to(centroid, tet_volume, tet_centroid);

    }

    _centroid = {
        centroid[0] / total_volume,
        centroid[1] / total_volume,
        centroid[2] / total_volume,
    };

    _volume = total_volume;

  }

  void
  update_box_min(const std::array<double, 3> &x) {

    if (x[0] < _box_min[0]) _box_min[0] = x[0];
    if (x[1] < _box_min[1]) _box_min[1] = x[1];
    if (x[2] < _box_min[2]) _box_min[2] = x[2];

  }

  void
  update_box_max(const std::array<double, 3> &x) {

    if (x[0] > _box_max[0]) _box_max[0] = x[0];
    if (x[1] > _box_max[1]) _box_max[1] = x[1];
    if (x[2] > _box_max[2]) _box_max[2] = x[2];

  }

};

#endif // MMPPT_TOY_QT_VTK_EX005_MESH_HPP_

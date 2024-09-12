//
// Created by Lesleis Nagy on 15/06/2024.
//

#ifndef MMPPT_TOY_QT_VTK_EX005_MODEL_HPP_
#define MMPPT_TOY_QT_VTK_EX005_MODEL_HPP_

#include <utility>

#include <vtkActor.h>
#include <vtkUnstructuredGrid.h>
#include <vtkProperty.h>
#include <vtkDataSetMapper.h>

#include "aliases.hpp"
#include "field.hpp"
#include "mesh.hpp"

/**
 * A model consists of a mesh and a field.
 */
class Model {

 public:

  Model(v_list vcl, tet_list til, sm_list sml) :
      _mesh{std::move(vcl),
            std::move(til),
            std::move(sml)} {}

  Model(v_list vcl, tet_list til, sm_list sml, FieldList field_list) :
      _mesh{std::move(vcl),
            std::move(til),
            std::move(sml)},
      _field_list{std::move(field_list)} {}

  [[nodiscard]] const Mesh &
  mesh() const;

  [[nodiscard]] const FieldList &
  field_list() const;

  void
  add_field(Field field);

  //--------------------------------------------------------------------------
  // VTK graphics related functions
  //--------------------------------------------------------------------------

  void
  enable_graphics();

  [[nodiscard]] bool
  graphics_enabled() const;

  [[nodiscard]] vtkSmartPointer<vtkActor>
  u_grid_actor() const;

  [[nodiscard]] std::array<double, 3>
  u_grid_center() const {

    double *gc = _u_grid->GetCenter();

    return {gc[0], gc[1], gc[2]};

  }

 private:

  Mesh _mesh;

  FieldList _field_list;

  // Graphics/display

  // Flag to indicate that graphics are enabled.
  bool _graphics_enabled{false};

  // Pointer to a VTK unstructured grid.
  vtkSmartPointer<vtkUnstructuredGrid> _u_grid;

  // Pointer to an unstructured grid dataset mapper.
  vtkSmartPointer<vtkDataSetMapper> _u_grid_ds_mapper;

  // Pointer to a VTK unstructured grid actor.
  vtkSmartPointer<vtkActor> _u_grid_actor;

  /**
   * Function to set up the unstructured grid associated with this mesh.
   */
  void
  setup_u_grid();

};

#endif // MMPPT_TOY_QT_VTK_EX005_MODEL_HPP_

//
// Created by Lesleis Nagy on 15/06/2024.
//

#ifndef MMPPT_TOY_QT_VTK_EX005_MODEL_HPP_
#define MMPPT_TOY_QT_VTK_EX005_MODEL_HPP_

#include <iomanip>
#include <utility>
#include <regex>
#include <sstream>

#include <vtkActor.h>
#include <vtkArrayCalculator.h>
#include <vtkDataSetMapper.h>
#include <vtkDoubleArray.h>
#include <vtkGradientFilter.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkUnstructuredGrid.h>

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
  ugrid_actor() const;

  [[nodiscard]] vtkSmartPointer<vtkDataSetMapper>
  ugrid_ds_mapper() const;

  [[nodiscard]] vtkSmartPointer<vtkUnstructuredGrid>
  ugrid() const;

  [[nodiscard]] std::string
  field_name(const std::string &prefix, int index) const;

  [[nodiscard]] std::optional<int>
  field_name_index(const std::string &name) const;

  void
  add_ugrid_actor(vtkSmartPointer<vtkRenderer> &renderer) const;

  void
  remove_ugrid_actor(vtkSmartPointer<vtkRenderer> &renderer) const;

 private:

  Mesh _mesh;

  FieldList _field_list;

  std::regex _regex_field_name{R"([a-zA-Z]([0-9]+))"};

  // Graphics/display

  // Flag to indicate that graphics are enabled.
  bool _graphics_enabled{false};

  // Pointer to a VTK unstructured grid.
  vtkSmartPointer<vtkUnstructuredGrid> _ugrid;

  // Pointer to an unstructured grid dataset mapper.
  vtkSmartPointer<vtkDataSetMapper> _ugrid_ds_mapper;

  // Pointer to a VTK unstructured grid actor.
  vtkSmartPointer<vtkActor> _ugrid_actor;

  // Field names zero-padding length.
  int _zero_pad_length{5};

  /**
   * Function to set up the unstructured grid associated with this mesh.
   */
  void
  setup_ugrid();

  /**
   * Function to set up magnetization vector data.
   */
  void
  setup_ugrid_fields();

  void
  setup_ugrid_field(int index, const Field &field);

  void
  setup_ugrid_vorticity(int index);

  void
  setup_ugrid_helicity(int index);

};

#endif // MMPPT_TOY_QT_VTK_EX005_MODEL_HPP_

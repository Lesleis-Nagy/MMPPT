//
// Created by Lesleis Nagy on 15/06/2024.
//

#ifndef MMPPT_TOY_QT_VTK_EX005_MODEL_HPP_
#define MMPPT_TOY_QT_VTK_EX005_MODEL_HPP_

#include <iomanip>
#include <regex>
#include <sstream>
#include <unordered_set>
#include <utility>
#include <optional>

#include <vtkActor.h>
#include <vtkArrayCalculator.h>
#include <vtkArrowSource.h>
#include <vtkDataSetMapper.h>
#include <vtkDoubleArray.h>
#include <vtkGlyph3D.h>
#include <vtkGradientFilter.h>
#include <vtkLookupTable.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkUnstructuredGrid.h>

#include "aliases.hpp"
#include "config_consts.h"
#include "field.hpp"
#include "mesh.hpp"
#include "palettes.hpp"

/**
 * A model consists of a mesh and a field.
 */
class Model {

 public:

  struct MinMax {
    double min;
    double max;
  };

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

  [[nodiscard]] std::unordered_map<std::string, MinMax>
  heli_minmax() const;

  [[nodiscard]] std::unordered_map<std::string, MinMax>
  rheli_minmax() const;

  [[nodiscard]] double
  length_scale() const;

  [[nodiscard]] std::array<double, 3>
  center() const;

  void
  add_ugrid_actor(vtkSmartPointer<vtkRenderer> &renderer) const;

  void
  remove_ugrid_actor(vtkSmartPointer<vtkRenderer> &renderer) const;

  void
  add_arrow_actor(vtkSmartPointer<vtkRenderer> &renderer) const;

  void
  remove_arrow_actor(vtkSmartPointer<vtkRenderer> &renderer) const;

  void
  set_ugrid_opacity(double opacity);

  void
  set_arrow_opacity(double opacity);

  void
  set_arrow_scale(double scale);

 private:

  Mesh _mesh;

  FieldList _field_list;

  std::regex _regex_field_name{R"([a-zA-Z]([0-9]+))"};

  // Graphics/display

  // Field names zero-padding length.
  int _zero_pad_length{5};

  // Flag to indicate that graphics are enabled.
  bool _graphics_enabled{false};

  // Field name.
  std::vector<std::string> _mag_names;
  std::vector<std::string> _vort_names;
  std::vector<std::string> _heli_names;
  std::vector<std::string> _rheli_names;

  // Field helicity min/max values
  std::unordered_map<std::string, MinMax> _heli_minmax;

  // Field relative helicity min/max value (absolute range [-1, 1]).
  std::unordered_map<std::string, MinMax> _rheli_minmax;

  // Pointer to a VTK unstructured grid.
  vtkSmartPointer<vtkUnstructuredGrid> _ugrid;

  // Pointer to an unstructured grid dataset mapper.
  vtkSmartPointer<vtkDataSetMapper> _ugrid_ds_mapper;

  // Pointer to a VTK unstructured grid actor.
  vtkSmartPointer<vtkActor> _ugrid_actor;

  // Arrow source.
  vtkSmartPointer<vtkArrowSource> _arrow_source;

  // Arrow transform.
  vtkSmartPointer<vtkTransform> _arrow_transform;

  // Arrow transform filter.
  vtkSmartPointer<vtkTransformPolyDataFilter> _arrow_transform_filter;

  // Arrow glyph.
  vtkSmartPointer<vtkGlyph3D> _arrow_glyph;

  // Arrow colour lookup table.
  vtkSmartPointer<vtkLookupTable> _arrow_colour_lookup_table;

  // Arrow poly data mapper
  vtkSmartPointer<vtkPolyDataMapper> _arrow_glyph_poly_data_mapper;

  // Arrow actor.
  vtkSmartPointer<vtkActor> _arrow_actor;

  // Arrow shaft/tip resolution.
  int _arrow_shaft_resolution{10};
  int _arrow_tip_resolution{30};

  // Arrow scale.
  double _arrow_scale{.005};

  /**
   * Function to set t_theta the unstructured grid associated with this mesh.
   */
  void
  setup_ugrid();

  /**
   * Function to set t_theta magnetization vector data.
   */
  void
  setup_ugrid_fields();

  /**
   * Unstructured grid.
   */
  void
  setup_ugrid_field(int index, const Field &field);

  /**
   * Unstructured grid calculations.
   */
  void
  setup_ugrid_calculations(int index);

  void
  setup_arrows();

  void
  setup_arrow_color_lookup_table(const std::string &name, double val_min, double val_max);

};

#endif // MMPPT_TOY_QT_VTK_EX005_MODEL_HPP_

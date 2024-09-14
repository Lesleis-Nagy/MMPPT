//
// Created by Lesleis Nagy on 04/09/2024.
//

#include "model.hpp"

//--------------------------------------------------------------------------
// Public functions.
//--------------------------------------------------------------------------

const Mesh &
Model::mesh() const {
  return _mesh;
}

const FieldList &
Model::field_list() const {
  return _field_list;
}

void
Model::add_field(Field field) {
  _field_list.add_field(std::move(field));
}

void Model::enable_graphics() {
  setup_ugrid();
  setup_ugrid_fields();
  setup_arrows();

  _ugrid->GetPointData()->SetActiveVectors(_mag_names[0].c_str());
  _ugrid->GetPointData()->SetActiveScalars(_rheli_names[0].c_str());

  _graphics_enabled = true;
}

bool Model::graphics_enabled() const {
  return _graphics_enabled;
}

vtkSmartPointer<vtkActor>
Model::ugrid_actor() const {
  return _ugrid_actor;
}

vtkSmartPointer<vtkDataSetMapper>
Model::ugrid_ds_mapper() const {
  return _ugrid_ds_mapper;
}

vtkSmartPointer<vtkUnstructuredGrid>
Model::ugrid() const {
  return _ugrid;
}

std::string
Model::field_name(const std::string &prefix, int index) const {
  std::stringstream ss_mag;
  ss_mag << prefix << std::setw(_zero_pad_length) << std::setfill('0') << index;
  return ss_mag.str();
}

std::optional<int>
Model::field_name_index(const std::string &name) const {
  std::smatch matches;
  if (std::regex_match(name, matches, _regex_field_name)) {
    return stoi(matches[1].str());
  }
  return std::nullopt;
}

std::unordered_map<std::string, Model::MinMax>
Model::heli_minmax() const {
  return _heli_minmax;
}

std::unordered_map<std::string, Model::MinMax>
Model::rheli_minmax() const {
  return _rheli_minmax;
}

double
Model::length_scale() const {

  double bbox[6];
  _ugrid_actor->GetBounds(bbox);

  return std::sqrt(
      pow(bbox[1] - bbox[0], 2.0)
          + pow(bbox[3] - bbox[2], 2.0)
          + pow(bbox[5] - bbox[4], 2.0)
  );

}

std::array<double, 3>
Model::center() const {

  double *center = _ugrid_actor->GetCenter();

  return {center[0], center[1], center[2]};

}

void
Model::add_ugrid_actor(vtkSmartPointer<vtkRenderer> &renderer) const {
  if (_ugrid_actor) {
    renderer->AddActor(_ugrid_actor);
  }
}

void
Model::remove_ugrid_actor(vtkSmartPointer<vtkRenderer> &renderer) const {
  if (_ugrid_actor) {
    renderer->RemoveActor(_ugrid_actor);
  }
}

void
Model::add_arrow_actor(vtkSmartPointer<vtkRenderer> &renderer) const {
  if (_arrow_actor) {
    renderer->AddActor(_arrow_actor);
  }
}

void
Model::remove_arrow_actor(vtkSmartPointer<vtkRenderer> &renderer) const {
  if (_arrow_actor) {
    renderer->RemoveActor(_arrow_actor);
  }
}

void
Model::set_ugrid_opacity(double opacity) {
  _ugrid_actor->GetProperty()->SetOpacity(opacity);
}

void
Model::set_arrow_opacity(double opacity) {
  _arrow_actor->GetProperty()->SetOpacity(opacity);
}

void
Model::set_arrow_scale(double scale) {
  _arrow_glyph->SetScaleFactor(scale * length_scale());
}



//--------------------------------------------------------------------------
// Private functions.
//--------------------------------------------------------------------------

void
Model::setup_ugrid() {

  // Create the unstructured grid.

  _ugrid = vtkUnstructuredGrid::New();

  // Add vertex points to the unstructured grid.
  vtkSmartPointer<vtkPoints> points = vtkPoints::New();
  points->SetNumberOfPoints((vtkIdType) _mesh.vcl().size());

  const auto &vcl = _mesh.vcl();
  for (vtkIdType i = 0; i < vcl.size(); ++i) {
    points->SetPoint(i, vcl[i][0], vcl[i][1], vcl[i][2]);
  }
  _ugrid->SetPoints(points);

  // Add connectivity information for unstructured grids.
  const auto &til = _mesh.til();
  for (vtkIdType i = 0; i < til.size(); ++i) {
    vtkIdType element[4] = {
        (vtkIdType) til[i][0],
        (vtkIdType) til[i][1],
        (vtkIdType) til[i][2],
        (vtkIdType) til[i][3]
    };
    _ugrid->InsertNextCell(VTK_TETRA, 4, element);
  }

  // Create the dataset mapper.
  _ugrid_ds_mapper = vtkDataSetMapper::New();
  _ugrid_ds_mapper->SetInputData(_ugrid);
  _ugrid_ds_mapper->ScalarVisibilityOff();

  // Create the actor.
  _ugrid_actor = vtkActor::New();
  _ugrid_actor->SetMapper(_ugrid_ds_mapper);

  //_u_grid_actor->GetProperty()->SetRepresentationToWireframe();
  _ugrid_actor->GetProperty()->SetRepresentationToSurface();
  _ugrid_actor->GetProperty()->EdgeVisibilityOn();
  _ugrid_actor->GetProperty()->SetEdgeColor(0, 0, 0);
  _ugrid_actor->GetProperty()->SetAmbient(1.0);
  _ugrid_actor->GetProperty()->SetDiffuse(0.0);
  _ugrid_actor->GetProperty()->SetSpecular(0.0);

}

void
Model::setup_ugrid_fields() {

  std::cout << "setup_ugrid_fields()" << std::endl;

  for (int i = 0; i < _field_list.n_fields(); ++i) {
    const auto &f = _field_list.fields()[i];
    setup_ugrid_field(i, f);
    setup_ugrid_calculations(i);
  }

}

void
Model::setup_ugrid_field(int index, const Field &field) {

  std::cout << "setup_ugrid_field()" << std::endl;

  const auto &vectors = field.vectors();

  vtkSmartPointer<vtkDoubleArray> f = vtkDoubleArray::New();
  f->SetName(field_name("m", index).c_str());
  f->SetNumberOfComponents(3);
  f->SetNumberOfTuples((vtkIdType) vectors.size());

  for (vtkIdType i = 0; i < vectors.size(); ++i) {
    double fd[3] = {vectors[i][0], vectors[i][1], vectors[i][2]};
    f->SetTuple(i, fd);
  }

  _ugrid->GetPointData()->AddArray(f);

}

void
Model::setup_ugrid_calculations(int index) {

  std::cout << "setup_ugrid_calculations()" << std::endl;

  vtkSmartPointer<vtkGradientFilter> vorticity = vtkGradientFilter::New();

  std::string mag_name = field_name("m", index);
  std::string vort_name = field_name("v", index);
  std::string heli_name = field_name("h", index);
  std::string rheli_name = field_name("rh", index);

  // Vorticity computation.

  vorticity->ComputeVorticityOn();
  vorticity->SetInputArrayToProcess(0, 0, 0, 0, mag_name.c_str());
  vorticity->SetVorticityArrayName(vort_name.c_str());
  vorticity->SetInputData(_ugrid);
  vorticity->Update();

  // Helicity computation.

  vtkSmartPointer<vtkArrayCalculator> helicity =
      vtkArrayCalculator::New();

  std::stringstream ss_h_func;
  ss_h_func << "dot(" << mag_name << ", " << vort_name << ")";

  helicity->SetAttributeTypeToPointData();
  helicity->AddVectorArrayName(mag_name.c_str());
  helicity->AddVectorArrayName(vort_name.c_str());
  helicity->SetResultArrayName(heli_name.c_str());
  helicity->SetFunction(ss_h_func.str().c_str());
  helicity->SetInputData(vorticity->GetOutput());
  helicity->Update();

  // Relative helicity computation.

  vtkSmartPointer<vtkArrayCalculator> relative_helicity =
      vtkArrayCalculator::New();

  std::stringstream ss_rh_func;
  ss_rh_func << "dot(" << mag_name << ", " << vort_name << ")";
  ss_rh_func << "/";
  ss_rh_func << "mag(" << vort_name << ")";

  relative_helicity->SetAttributeTypeToPointData();
  relative_helicity->AddVectorArrayName(mag_name.c_str());
  relative_helicity->AddVectorArrayName(vort_name.c_str());
  relative_helicity->SetResultArrayName(rheli_name.c_str());
  relative_helicity->SetFunction(ss_rh_func.str().c_str());
  relative_helicity->SetInputData(vorticity->GetOutput());
  relative_helicity->Update();

  // Add helicity field

  vtkUnstructuredGrid *hug =
      vtkUnstructuredGrid::SafeDownCast(helicity->GetOutput());

  vtkSmartPointer<vtkDoubleArray> hug_array =
      vtkDoubleArray::SafeDownCast(
          hug->GetPointData()->GetArray(heli_name.c_str())
      );

  vtkSmartPointer<vtkDoubleArray> hug_darray = vtkDoubleArray::New();
  hug_darray->SetName(heli_name.c_str());
  hug_darray->SetNumberOfComponents(1);
  hug_darray->SetNumberOfTuples(hug_array->GetNumberOfTuples());

  for (vtkIdType i = 0; i < hug_array->GetNumberOfTuples(); ++i) {
    hug_darray->SetValue(i, hug_array->GetTuple(i)[0]);
  }
  double hrange[2];
  hug_darray->GetRange(hrange);
  _ugrid->GetPointData()->AddArray(hug_darray);
  _heli_minmax[heli_name] = {.min = hrange[0], .max = hrange[1]};

  // Add relative helicity field

  vtkUnstructuredGrid *rhug =
      vtkUnstructuredGrid::SafeDownCast(relative_helicity->GetOutput());

  vtkSmartPointer<vtkDoubleArray> rhug_array =
      vtkDoubleArray::SafeDownCast(
          rhug->GetPointData()->GetArray(rheli_name.c_str())
      );

  vtkSmartPointer<vtkDoubleArray> rhug_darray = vtkDoubleArray::New();
  rhug_darray->SetName(rheli_name.c_str());
  rhug_darray->SetNumberOfComponents(1);
  rhug_darray->SetNumberOfTuples(rhug_array->GetNumberOfTuples());

  for (vtkIdType i = 0; i < rhug_array->GetNumberOfTuples(); ++i) {
    rhug_darray->SetValue(i, rhug_array->GetTuple(i)[0]);
  }
  double rhrange[2];
  rhug_darray->GetRange(rhrange);
  _ugrid->GetPointData()->AddArray(rhug_darray);
  _rheli_minmax[rheli_name] = {.min = rhrange[0], .max = rhrange[1]};

  // Push back the names for magnetization, vorticity, helicity and relative
  // helicity.
  _mag_names.push_back(mag_name);
  _vort_names.push_back(vort_name);
  _heli_names.push_back(heli_name);
  _rheli_names.push_back(rheli_name);

}

void
Model::setup_arrows() {

  QSettings settings;
  double val_min = -1.0;
  double val_max = 1.0;

  _arrow_source = vtkArrowSource::New();
  _arrow_source->SetShaftResolution(_arrow_shaft_resolution);
  _arrow_source->SetTipResolution(_arrow_tip_resolution);

  _arrow_transform = vtkTransform::New();
  _arrow_transform->Translate(-0.5, 0.0, 0.0);

  _arrow_transform_filter = vtkTransformPolyDataFilter::New();
  _arrow_transform_filter->SetTransform(_arrow_transform);
  _arrow_transform_filter->SetInputConnection(_arrow_source->GetOutputPort());

  _arrow_glyph = vtkGlyph3D::New();
  _arrow_glyph->SetInputData(_ugrid);
  _arrow_glyph->SetSourceConnection(_arrow_transform_filter->GetOutputPort());
  _arrow_glyph->SetScaleModeToScaleByVector();
  _arrow_glyph->SetVectorModeToUseVector();
  _arrow_glyph->SetColorModeToColorByScalar();
  _arrow_glyph->ScalingOn();
  _arrow_glyph->OrientOn();
  _arrow_glyph->SetScaleFactor(_arrow_scale);
  _arrow_glyph->Update();

  _arrow_glyph_poly_data_mapper = vtkPolyDataMapper::New();
  _arrow_glyph_poly_data_mapper->SetInputConnection(
      _arrow_glyph->GetOutputPort()
  );

  setup_arrow_color_lookup_table(
      settings.value(CONFIG_CURRENT_GEOMETRY_COLOR_SCHEME, "accent")
          .toString()
          .toStdString(),
      val_min, val_max
  );

  _arrow_glyph_poly_data_mapper->SetScalarVisibility(true);
  _arrow_glyph_poly_data_mapper->SetColorModeToMapScalars();
  _arrow_glyph_poly_data_mapper->SetLookupTable(_arrow_colour_lookup_table);
  _arrow_glyph_poly_data_mapper->UseLookupTableScalarRangeOn();
  //_arrow_glyph_poly_data_mapper->SetScalarRange(val_min, val_max);
  _arrow_glyph_poly_data_mapper->Update();

  _arrow_actor = vtkActor::New();
  _arrow_actor->SetMapper(_arrow_glyph_poly_data_mapper);

}

void
Model::setup_arrow_color_lookup_table(const std::string &name,
                                      double val_min,
                                      double val_max) {

  if (colormap::palettes.find(name) == colormap::palettes.end()) {
    return;
  }

  auto color_fn = colormap::palettes.at(name).rescale(val_min, val_max);

  vtkIdType n = 256;
  double dval = (val_max - val_min) / (double) (n - 1);

  _arrow_colour_lookup_table = vtkLookupTable::New();
  _arrow_colour_lookup_table->SetTableRange(val_min, val_max);
  _arrow_colour_lookup_table->SetNumberOfTableValues(n);

  for (vtkIdType i = 0; i < n; ++i) {
    double val = val_min + (double) i * dval;
    auto color = color_fn(val);
    double r = (double) color.getRed().getValue() / 255.0;
    double g = (double) color.getGreen().getValue() / 255.0;
    double b = (double) color.getBlue().getValue() / 255.0;
    double a = 1.0;
    std::cout << "Setting color for val (" << val << ") => ";
    std::cout << "r: " << r << ", ";
    std::cout << "g: " << g << ", ";
    std::cout << "b: " << b << ", ";
    std::cout << "a: " << a << std::endl;
    _arrow_colour_lookup_table->SetTableValue(i, r, g, b, a);
  }

  _arrow_colour_lookup_table->Build();

}
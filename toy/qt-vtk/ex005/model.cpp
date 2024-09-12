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

//--------------------------------------------------------------------------
// Private functions.
//--------------------------------------------------------------------------

void Model::setup_ugrid() {

  // Create the unstructured grid.

  _ugrid = vtkUnstructuredGrid::New();

  // Add vertex points to the unstructured grid.
  vtkSmartPointer<vtkPoints> points = vtkPoints::New();
  points->SetNumberOfPoints((vtkIdType)_mesh.vcl().size());

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

void Model::setup_ugrid_fields() {

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
  f->SetNumberOfTuples((vtkIdType)vectors.size());

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

  std::string m = field_name("m", index);
  std::string v = field_name("v", index);
  std::string h = field_name("h", index);
  std::string rh = field_name("rh", index);

  // Vorticity computation.

  vorticity->ComputeVorticityOn();
  vorticity->SetInputArrayToProcess(0, 0, 0, 0, m.c_str());
  vorticity->SetVorticityArrayName(v.c_str());
  vorticity->SetInputData(_ugrid);
  vorticity->Update();

  // Helicity computation.

  vtkSmartPointer<vtkArrayCalculator> helicity =
      vtkArrayCalculator::New();

  std::stringstream ss_h_func;
  ss_h_func << "dot(" << m << ", " << v << ")";

  helicity->SetAttributeTypeToPointData();
  helicity->AddVectorArrayName(m.c_str());
  helicity->AddVectorArrayName(v.c_str());
  helicity->SetResultArrayName(h.c_str());
  helicity->SetFunction(ss_h_func.str().c_str());
  helicity->SetInputData(vorticity->GetOutput());
  helicity->Update();

  // Relative helicity computation.

  vtkSmartPointer<vtkArrayCalculator> relative_helicity =
      vtkArrayCalculator::New();

  std::stringstream ss_rh_func;
  ss_rh_func << "dot(" << m << ", " << v << ") / mag(" << v << ")";

  relative_helicity->SetAttributeTypeToPointData();
  relative_helicity->AddVectorArrayName(m.c_str());
  relative_helicity->AddVectorArrayName(v.c_str());
  relative_helicity->SetResultArrayName(rh.c_str());
  relative_helicity->SetFunction(ss_rh_func.str().c_str());
  relative_helicity->SetInputData(vorticity->GetOutput());
  relative_helicity->Update();

  // Add helicity field

  vtkUnstructuredGrid *hug =
      vtkUnstructuredGrid::SafeDownCast(helicity->GetOutput());

  vtkSmartPointer<vtkDoubleArray> hug_array =
      vtkDoubleArray::SafeDownCast(hug->GetPointData()->GetArray(h.c_str()));

  vtkSmartPointer<vtkDoubleArray> hug_darray = vtkDoubleArray::New();
  hug_darray->SetName(h.c_str());
  hug_darray->SetNumberOfComponents(1);
  hug_darray->SetNumberOfTuples(hug_array->GetNumberOfTuples());

  for (vtkIdType i = 0; i < hug_array->GetNumberOfTuples(); ++i) {
    hug_darray->SetValue(i, hug_array->GetTuple(i)[0]);
  }
  _ugrid->GetPointData()->AddArray(hug_darray);

  // Add relative helicity field

  vtkUnstructuredGrid *rhug =
      vtkUnstructuredGrid::SafeDownCast(relative_helicity->GetOutput());

  vtkSmartPointer<vtkDoubleArray> rhug_array =
      vtkDoubleArray::SafeDownCast(rhug->GetPointData()->GetArray(rh.c_str()));

  vtkSmartPointer<vtkDoubleArray> rhug_darray = vtkDoubleArray::New();
  rhug_darray->SetName(rh.c_str());
  rhug_darray->SetNumberOfComponents(1);
  rhug_darray->SetNumberOfTuples(rhug_array->GetNumberOfTuples());

  for (vtkIdType i = 0; i < rhug_array->GetNumberOfTuples(); ++i) {
    rhug_darray->SetValue(i, rhug_array->GetTuple(i)[0]);
  }
  _ugrid->GetPointData()->AddArray(rhug_darray);

}


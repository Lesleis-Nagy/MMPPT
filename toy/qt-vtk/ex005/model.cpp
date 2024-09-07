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
  setup_u_grid();
  _graphics_enabled = true;
}

bool Model::graphics_enabled() const {
  return _graphics_enabled;
}

vtkSmartPointer<vtkActor>
Model::u_grid_actor() const {
    return _u_grid_actor;
}

//--------------------------------------------------------------------------
// Private functions.
//--------------------------------------------------------------------------

void Model::setup_u_grid() {

  // Create the unstructured grid.

  _u_grid = vtkUnstructuredGrid::New();

  // Add vertex points to the unstructured grid.
  vtkSmartPointer<vtkPoints> points = vtkPoints::New();
  points->SetNumberOfPoints(_mesh.vcl().size());

  const auto &vcl = _mesh.vcl();
  for (vtkIdType i = 0; i < vcl.size(); ++i) {
    points->SetPoint(i, vcl[i][0], vcl[i][1], vcl[i][2]);
  }
  _u_grid->SetPoints(points);

  // Add connectivity information for unstructured grids.
  const auto &til = _mesh.til();
  for (vtkIdType i = 0; i < til.size(); ++i) {
    vtkIdType element[4] = {
        (vtkIdType)til[i][0],
        (vtkIdType)til[i][1],
        (vtkIdType)til[i][2],
        (vtkIdType)til[i][3]
    };
    _u_grid->InsertNextCell(VTK_TETRA, 4, element);
  }

  // Create the dataset mapper.
  _u_grid_ds_mapper = vtkDataSetMapper::New();
  _u_grid_ds_mapper->SetInputData(_u_grid);
  _u_grid_ds_mapper->ScalarVisibilityOff();

  // Create the actor.
  _u_grid_actor = vtkActor::New();
  _u_grid_actor->SetMapper(_u_grid_ds_mapper);

  //_u_grid_actor->GetProperty()->SetRepresentationToWireframe();
  _u_grid_actor->GetProperty()->SetRepresentationToSurface();
  _u_grid_actor->GetProperty()->EdgeVisibilityOn();
  _u_grid_actor->GetProperty()->SetEdgeColor(0,0,0);
  _u_grid_actor->GetProperty()->SetAmbient(1.0);
  _u_grid_actor->GetProperty()->SetDiffuse(0.0);
  _u_grid_actor->GetProperty()->SetSpecular(0.0);

}

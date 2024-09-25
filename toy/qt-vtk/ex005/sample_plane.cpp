//
// Created by Lesleis Nagy on 17/09/2024.
//

#include "sample_plane.hpp"

// Macro to convert radians to degrees
#define R2D(x) ((x) * (180.0 / M_PI))

// Macro to convert degrees to radians
#define D2R(x) ((x) * (M_PI / 180.0))

SamplePlane::SamplePlane(double length_scale) :
    _length_scale{length_scale},
    _scale_multiplier{0.05},
    _point_resolution_theta{30},
    _point_resolution_phi{30},
    _target{0.0, 0.0, 0.0},
    _n{0.0, 0.0, 0.0},
    _t_theta{0.0, 0.0, 0.0},
    _t_phi{0.0, 0.0, 0.0},
    _theta{0.0},
    _phi{0.0},
    _gamma{0.0},
    _pc{0.0, 0.0, 0.0},
    _p1{0.0, 0.0, 0.0},
    _p2{0.0, 0.0, 0.0},
    _p3{0.0, 0.0, 0.0},
    _p4{0.0, 0.0, 0.0},
    _rot_matrix{{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}},
    _sample_point1_source{nullptr},
    _sample_point1_data_mapper{nullptr},
    _sample_point1_actor{nullptr},
    _sample_point2_source{nullptr},
    _sample_point2_data_mapper{nullptr},
    _sample_point2_actor{nullptr},
    _sample_point3_source{nullptr},
    _sample_point3_data_mapper{nullptr},
    _sample_point3_actor{nullptr},
    _sample_point4_source{nullptr},
    _sample_point4_data_mapper{nullptr},
    _sample_point4_actor{nullptr} {

  _r = 2.0 * _length_scale;
  _width = _length_scale;
  _height = _length_scale;

  create_vtk_objects();
  update_sample_points();

}

//---------------------------------------------------------------------------//
//- Public getters                                                          -//
//---------------------------------------------------------------------------//

double
SamplePlane::length_scale() const {

  return _length_scale;

}

double
SamplePlane::scale_multiplier() const {

  return _scale_multiplier;

}

int
SamplePlane::point_resolution_theta() const {

  return _point_resolution_theta;

}

int
SamplePlane::point_resolution_phi() const {

  return _point_resolution_phi;

}

const lcgl::Vector3D<double> &
SamplePlane::target() const {

  return _target;

}

const lcgl::Vector3D<double> &
SamplePlane::pc() const {

  return _pc;

}

const lcgl::Vector3D<double> &
SamplePlane::p1() const {

  return _p1;

}

const lcgl::Vector3D<double> &
SamplePlane::p2() const {

  return _p2;

}

const lcgl::Vector3D<double> &
SamplePlane::p3() const {

  return _p3;

}

const lcgl::Vector3D<double> &
SamplePlane::p4() const {

  return _p4;

}

const lcgl::Vector3D<double> &
SamplePlane::n() const {

  return _n;

}

const lcgl::Vector3D<double> &
SamplePlane::t_theta() const {

  return _t_theta;

}

const lcgl::Vector3D<double> &
SamplePlane::t_phi() const {

  return _t_phi;

}

double
SamplePlane::width() const {

  return _width;

}

double
SamplePlane::height() const {

  return _height;

}

double
SamplePlane::theta() const {

  return _theta;

}

double
SamplePlane::phi() const {

  return _phi;

}

double
SamplePlane::gamma() const {

  return _gamma;

}

double
SamplePlane::r() const {

  return _r;

}

//-------------------------------------------------------------------------//
// Public utility functions
//-------------------------------------------------------------------------//

void
SamplePlane::add_actors_to_renderer(
    vtkSmartPointer<vtkRenderer> &renderer
) const {

  renderer->AddActor(_sample_point1_actor);
  renderer->AddActor(_sample_point2_actor);
  renderer->AddActor(_sample_point3_actor);
  renderer->AddActor(_sample_point4_actor);

}

void
SamplePlane::remove_actors_from_renderer(
    vtkSmartPointer<vtkRenderer> &renderer
) const {

  renderer->RemoveActor(_sample_point1_actor);
  renderer->RemoveActor(_sample_point2_actor);
  renderer->RemoveActor(_sample_point3_actor);
  renderer->RemoveActor(_sample_point4_actor);

}

bool
SamplePlane::have_sample_point1_vtk_objects() const {

  return _sample_point1_source != nullptr
      && _sample_point1_data_mapper != nullptr
      && _sample_point1_actor != nullptr;

}

bool
SamplePlane::have_sample_point2_vtk_objects() const {

  return _sample_point2_source != nullptr
      && _sample_point2_data_mapper != nullptr
      && _sample_point2_actor != nullptr;

}

bool
SamplePlane::have_sample_point3_vtk_objects() const {

  return _sample_point3_source != nullptr
      && _sample_point3_data_mapper != nullptr
      && _sample_point3_actor != nullptr;

}

bool
SamplePlane::have_sample_point4_vtk_objects() const {

  return _sample_point4_source != nullptr
      && _sample_point4_data_mapper != nullptr
      && _sample_point4_actor != nullptr;

}

//---------------------------------------------------------------------------//
//- Public setters                                                          -//
//---------------------------------------------------------------------------//

void
SamplePlane::update_sample_points() {

  update_n();
  update_pc();
  update_rot_matrix();
  update_t_theta();
  update_t_phi();

  update_p1();
  update_p2();
  update_p3();
  update_p4();

  update_sample_point1_actor();
  update_sample_point2_actor();
  update_sample_point3_actor();
  update_sample_point4_actor();

}

void
SamplePlane::scale_multiplier(double value) {

  _scale_multiplier = value;
  update_sample_points();

}

void
SamplePlane::point_resolution_theta(int value) {

  _point_resolution_theta = value;
  update_sample_points();

}

void
SamplePlane::point_resolution_phi(int value) {

  _point_resolution_phi = value;
  update_sample_points();

}

void
SamplePlane::target(lcgl::Vector3D<double> value) {

  _target = value;

}

void
SamplePlane::width(double value) {

  _width = value;
  update_sample_points();

}

void
SamplePlane::height(double value) {

  _height = value;
  update_sample_points();

}

void
SamplePlane::theta(double value) {

  _theta = value;
  update_sample_points();

}

void
SamplePlane::phi(double value) {

  _phi = value;
  update_sample_points();

}

void
SamplePlane::gamma(double value) {

  _gamma = value;
  update_sample_points();

}

void
SamplePlane::r(double value) {

  _r = value;
  update_sample_points();

}

void
SamplePlane::create_vtk_objects() {

  create_sample_point1_source();
  create_sample_point1_data_mapper();
  create_sample_point1_actor();

  create_sample_point2_source();
  create_sample_point2_data_mapper();
  create_sample_point2_actor();

  create_sample_point3_source();
  create_sample_point3_data_mapper();
  create_sample_point3_actor();

  create_sample_point4_source();
  create_sample_point4_data_mapper();
  create_sample_point4_actor();

}

void
SamplePlane::create_sample_point1_source() {

  _sample_point1_source = vtkSphereSource::New();
  update_sample_point1_source();

}

void
SamplePlane::update_sample_point1_source() {

  if (_sample_point1_source == nullptr) return;

  _sample_point1_source->SetCenter(_p1.x(), _p1.y(), _p2.z());
  _sample_point1_source->SetRadius(_scale_multiplier * _length_scale);
  _sample_point1_source->SetThetaResolution(_point_resolution_theta);
  _sample_point1_source->SetPhiResolution(_point_resolution_phi);
  _sample_point1_source->Update();

}

void
SamplePlane::create_sample_point1_data_mapper() {

  _sample_point1_data_mapper = vtkPolyDataMapper::New();
  _sample_point1_data_mapper->SetInputData(_sample_point1_source->GetOutput());

}

void
SamplePlane::create_sample_point1_actor() {

  _sample_point1_actor = vtkActor::New();
  _sample_point1_actor->SetMapper(_sample_point1_data_mapper);
  _sample_point1_actor->GetProperty()->SetColor(1, 0, 0);
  _sample_point1_actor->GetProperty()->LightingOff();

}

void
SamplePlane::update_sample_point1_actor() {

  if (_sample_point1_actor == nullptr) return;

  _sample_point1_actor->SetPosition(_p1.x(), _p1.y(), _p1.z());

}

void
SamplePlane::create_sample_point2_source() {

  _sample_point2_source = vtkSphereSource::New();
  update_sample_point2_source();

}

void
SamplePlane::update_sample_point2_source() {

  if (_sample_point2_source == nullptr) return;

  _sample_point2_source->SetCenter(_p2.x(), _p2.y(), _p2.z());
  _sample_point2_source->SetRadius(_scale_multiplier * _length_scale);
  _sample_point2_source->SetThetaResolution(_point_resolution_theta);
  _sample_point2_source->SetPhiResolution(_point_resolution_phi);
  _sample_point2_source->Update();

}

void
SamplePlane::create_sample_point2_data_mapper() {

  _sample_point2_data_mapper = vtkPolyDataMapper::New();
  _sample_point2_data_mapper->SetInputData(_sample_point2_source->GetOutput());

}

void
SamplePlane::create_sample_point2_actor() {

  _sample_point2_actor = vtkActor::New();
  _sample_point2_actor->SetMapper(_sample_point2_data_mapper);
  _sample_point2_actor->GetProperty()->SetColor(0, 1, 0);
  _sample_point2_actor->GetProperty()->LightingOff();

}

void
SamplePlane::update_sample_point2_actor() {

  if (_sample_point2_actor == nullptr) return;

  _sample_point2_actor->SetPosition(_p2.x(), _p2.y(), _p2.z());

}

void
SamplePlane::create_sample_point3_source() {

  _sample_point3_source = vtkSphereSource::New();
  update_sample_point3_source();

}

void
SamplePlane::update_sample_point3_source() {

  if (_sample_point3_source == nullptr) return;

  _sample_point3_source->SetCenter(_p3.x(), _p3.y(), _p3.z());
  _sample_point3_source->SetRadius(_scale_multiplier * _length_scale);
  _sample_point3_source->SetThetaResolution(_point_resolution_theta);
  _sample_point3_source->SetPhiResolution(_point_resolution_phi);
  _sample_point3_source->Update();

}

void
SamplePlane::create_sample_point3_data_mapper() {

  _sample_point3_data_mapper = vtkPolyDataMapper::New();
  _sample_point3_data_mapper->SetInputData(_sample_point3_source->GetOutput());

}

void
SamplePlane::create_sample_point3_actor() {

  _sample_point3_actor = vtkActor::New();
  _sample_point3_actor->SetMapper(_sample_point3_data_mapper);
  _sample_point3_actor->GetProperty()->SetColor(0, 0, 1);
  _sample_point3_actor->GetProperty()->LightingOff();

}

void
SamplePlane::update_sample_point3_actor() {

  if (_sample_point3_actor == nullptr) return;

  _sample_point3_actor->SetPosition(_p3.x(), _p3.y(), _p3.z());

}

void
SamplePlane::create_sample_point4_source() {

  _sample_point4_source = vtkSphereSource::New();
  update_sample_point4_source();

}

void
SamplePlane::update_sample_point4_source() {

  if (_sample_point4_source == nullptr) return;

  _sample_point4_source->SetCenter(_p4.x(), _p4.y(), _p4.z());
  _sample_point4_source->SetRadius(_scale_multiplier * _length_scale);
  _sample_point4_source->SetThetaResolution(_point_resolution_theta);
  _sample_point4_source->SetPhiResolution(_point_resolution_phi);
  _sample_point4_source->Update();

}

void
SamplePlane::create_sample_point4_data_mapper() {

  _sample_point4_data_mapper = vtkPolyDataMapper::New();
  _sample_point4_data_mapper->SetInputData(_sample_point4_source->GetOutput());

}

void
SamplePlane::create_sample_point4_actor() {

  _sample_point4_actor = vtkActor::New();
  _sample_point4_actor->SetMapper(_sample_point4_data_mapper);
  _sample_point4_actor->GetProperty()->SetColor(1, 1, 1);
  _sample_point4_actor->GetProperty()->LightingOff();

}

void
SamplePlane::update_sample_point4_actor() {

  if (_sample_point4_actor == nullptr) return;

  _sample_point4_actor->SetPosition(_p4.x(), _p4.y(), _p4.z());

}

const lcgl::Vector3D<double> &
SamplePlane::update_t_theta() {

  _t_theta = {
      cos(D2R(_theta)) * cos(D2R(_phi)),
      cos(D2R(_theta)) * sin(D2R(_phi)),
      -sin(D2R(_theta))
  };

  _t_theta = _rot_matrix * _t_theta;

  return _t_theta;

}

const lcgl::Vector3D<double> &
SamplePlane::update_t_phi() {

  _t_phi = {
      -sin(D2R(_phi)),
      cos(D2R(_phi)),
      0.0
  };

  _t_phi = _rot_matrix * _t_phi;

  return _t_phi;

}

const lcgl::Vector3D<double> &
SamplePlane::update_n() {

  _n = {
      sin(D2R(_theta)) * cos(D2R(_phi)),
      sin(D2R(_theta)) * sin(D2R(_phi)),
      cos(D2R(_theta))
  };
  return _n;

}

const lcgl::Vector3D<double> &
SamplePlane::update_pc() {

  _pc = {
      _r * sin(D2R(_theta)) * cos(D2R(_phi)),
      _r * sin(D2R(_theta)) * sin(D2R(_phi)),
      _r * cos(D2R(_theta))
  };
  return _pc;

}

const lcgl::Vector3D<double> &
SamplePlane::update_p1() {

  _p1 = _pc - (_height * _t_theta) / 2.0 - (_width * _t_phi) / 2.0;
  return _p1;

}

const lcgl::Vector3D<double> &
SamplePlane::update_p2() {

  _p2 = _pc - (_height * _t_theta) / 2.0 + (_width * _t_phi) / 2.0;
  return _p2;

}

const lcgl::Vector3D<double> &
SamplePlane::update_p3() {

  _p3 = _pc + (_height * _t_theta) / 2.0 - (_width * _t_phi) / 2.0;
  return _p3;

}

const lcgl::Vector3D<double> &
SamplePlane::update_p4() {

  _p4 = _pc + (_height * _t_theta) / 2.0 + (_width * _t_phi) / 2.0;
  return _p4;

}

const lcgl::Matrix3x3<double> &
SamplePlane::update_rot_matrix() {

  _rot_matrix = lcgl::rotation(D2R(_gamma), _n);

  return _rot_matrix;

}

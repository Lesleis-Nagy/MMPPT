//
// Created by Lesleis Nagy on 17/09/2024.
//

#include "sample_plane.hpp"

SamplePlane::SamplePlane(double length_scale) :
    _length_scale{length_scale},
    _scale_multiplier{0.05},
    _theta_resolution{30},
    _phi_resolution{30},
    _p1{0.0, 0.0, 0.0},
    _p2{0.0, 0.0, 0.0},
    _p3{0.0, 0.0, 0.0},
    _p4{0.0, 0.0, 0.0},
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

  _p1 = {0.0, 0.0, 0.0};

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
SamplePlane::theta_resolution() const {

  return _theta_resolution;

}

int
SamplePlane::phi_resolution() const {

  return _phi_resolution;

}

const lcgl::Vector3D<double> &
SamplePlane::target() const {

  return _target;

}

const lcgl::Vector3D<double> &
SamplePlane::r0() const {

  return _r0;

}

const lcgl::Vector3D<double> &
SamplePlane::n() const {

  return _n;

}

const lcgl::Vector3D<double> &
SamplePlane::up() const {

  return _up;

}

double
SamplePlane::width() const {

  return _width;

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

//---------------------------------------------------------------------------//
//- Public setters                                                          -//
//---------------------------------------------------------------------------//

void
SamplePlane::update_sample_points() {

  update_sample_point1_source();
  update_sample_point2_source();
  update_sample_point3_source();
  update_sample_point4_source();

}

void
SamplePlane::scale_multiplier(double value) {

  _scale_multiplier = value;
  update_sample_points();

}

void
SamplePlane::theta_resolution(int value) {

  _theta_resolution = value;
  update_sample_points();

}

void
SamplePlane::phi_resolution(int value) {

  _phi_resolution = value;
  update_sample_points();

}

void
SamplePlane::target(lcgl::Vector3D<double> value) {

  _target = value;

}


void
SamplePlane::r0(lcgl::Vector3D<double> value) {

  _r0 = value;

}

void
SamplePlane::width(double value) {

  _width = value;

}

void
SamplePlane::theta(double value) {

  _theta = value;

}

void
SamplePlane::phi(double value) {

  _phi = value;

}

void
SamplePlane::gamma(double value) {

  _gamma = value;

}

void
SamplePlane::r(double value) {

  _r = value;

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
  _sample_point1_source->SetThetaResolution(_theta_resolution);
  _sample_point1_source->SetPhiResolution(_phi_resolution);
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
SamplePlane::create_sample_point2_source() {

  _sample_point2_source = vtkSphereSource::New();
  update_sample_point2_source();

}

void
SamplePlane::update_sample_point2_source() {

  if (_sample_point2_source == nullptr) return;

  _sample_point2_source->SetCenter(_p2.x(), _p2.y(), _p2.z());
  _sample_point2_source->SetRadius(_scale_multiplier * _length_scale);
  _sample_point2_source->SetThetaResolution(_theta_resolution);
  _sample_point2_source->SetPhiResolution(_phi_resolution);
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
SamplePlane::create_sample_point3_source() {

  _sample_point3_source = vtkSphereSource::New();
  update_sample_point3_source();

}

void
SamplePlane::update_sample_point3_source() {

  if (_sample_point3_source == nullptr) return;

  _sample_point3_source->SetCenter(_p3.x(), _p3.y(), _p3.z());
  _sample_point3_source->SetRadius(_scale_multiplier * _length_scale);
  _sample_point3_source->SetThetaResolution(_theta_resolution);
  _sample_point3_source->SetPhiResolution(_phi_resolution);
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
SamplePlane::create_sample_point4_source() {

  _sample_point4_source = vtkSphereSource::New();
  update_sample_point4_source();

}

void
SamplePlane::update_sample_point4_source() {

  if (_sample_point4_source == nullptr) return;

  _sample_point4_source->SetCenter(_p4.x(), _p4.y(), _p4.z());
  _sample_point4_source->SetRadius(_scale_multiplier * _length_scale);
  _sample_point4_source->SetThetaResolution(_theta_resolution);
  _sample_point4_source->SetPhiResolution(_phi_resolution);
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
SamplePlane::calculate_n() {

  // The plane normal always points along the radial vector.
  _n = {
      sin(_theta)*cos(_phi),
      sin(_theta)*sin(_phi),
      cos(_theta)
  };

}

void
SamplePlane::update_p1() {

  _p1 = {



  };

}
void
SamplePlane::update_p2() {

}
void
SamplePlane::update_p3() {

}
void
SamplePlane::update_p4() {

}

//
// Created by Lesleis Nagy on 17/09/2024.
//

#ifndef MMPPT_TOY_QT_VTK_EX005_SAMPLE_PLANE_HPP_
#define MMPPT_TOY_QT_VTK_EX005_SAMPLE_PLANE_HPP_

#include <vtkCamera.h>
#include <vtkCoordinate.h>
#include <vtkCylinderSource.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkObjectFactory.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkTransform.h>

#include "model.hpp"

#include "lcgl.hpp"

class SamplePlane {

 public:

  SamplePlane(double length_scale);


  //-------------------------------------------------------------------------//
  //- Public getters                                                        -//
  //-------------------------------------------------------------------------//

  [[nodiscard]] double length_scale() const;

  [[nodiscard]] double scale_multiplier() const;

  [[nodiscard]] int theta_resolution() const;

  [[nodiscard]] int phi_resolution() const;

  [[nodiscard]] const lcgl::Vector3D<double> & target() const;

  [[nodiscard]] const lcgl::Vector3D<double> & r0() const;

  [[nodiscard]] const lcgl::Vector3D<double> & n() const;

  [[nodiscard]] const lcgl::Vector3D<double> & up() const;

  [[nodiscard]] double width() const;

  [[nodiscard]] double theta() const;

  [[nodiscard]] double phi() const;

  [[nodiscard]] double gamma() const;

  [[nodiscard]] double r() const;

  //-------------------------------------------------------------------------//
  //- Public setters                                                        -//
  //-------------------------------------------------------------------------//

  void update_sample_points();

  void scale_multiplier(double value);

  void theta_resolution(int value);

  void phi_resolution(int value);

  void target(lcgl::Vector3D<double> value);

  void r0(lcgl::Vector3D<double> value);

  void width(double value);

  void theta(double value);

  void phi(double value);

  void gamma(double value);

  void r(double value);

 private:

  double _length_scale;
  double _scale_multiplier;
  int _theta_resolution;
  int _phi_resolution;

  lcgl::Vector3D<double> _target;
  lcgl::Vector3D<double> _r0;
  lcgl::Vector3D<double> _n;
  lcgl::Vector3D<double> _up;
  double _theta; // polar angle
  double _phi;   // azimuthal angle
  double _gamma; // orientation angle
  double _r;     // distance from target.
  double _width;

  lcgl::Vector3D<double> _p1;
  lcgl::Vector3D<double> _p2;
  lcgl::Vector3D<double> _p3;
  lcgl::Vector3D<double> _p4;

  vtkSmartPointer<vtkSphereSource> _sample_point1_source;
  vtkSmartPointer<vtkPolyDataMapper> _sample_point1_data_mapper;
  vtkSmartPointer<vtkActor> _sample_point1_actor;

  vtkSmartPointer<vtkSphereSource> _sample_point2_source;
  vtkSmartPointer<vtkPolyDataMapper> _sample_point2_data_mapper;
  vtkSmartPointer<vtkActor> _sample_point2_actor;

  vtkSmartPointer<vtkSphereSource> _sample_point3_source;
  vtkSmartPointer<vtkPolyDataMapper> _sample_point3_data_mapper;
  vtkSmartPointer<vtkActor> _sample_point3_actor;

  vtkSmartPointer<vtkSphereSource> _sample_point4_source;
  vtkSmartPointer<vtkPolyDataMapper> _sample_point4_data_mapper;
  vtkSmartPointer<vtkActor> _sample_point4_actor;

  void
  create_vtk_objects();

  void
  create_sample_point1_source();

  void
  update_sample_point1_source();

  void
  create_sample_point1_data_mapper();

  void
  create_sample_point1_actor();

  void
  create_sample_point2_source();

  void
  update_sample_point2_source();

  void
  create_sample_point2_data_mapper();

  void
  create_sample_point2_actor();

  void
  create_sample_point3_source();

  void
  update_sample_point3_source();

  void
  create_sample_point3_data_mapper();

  void
  create_sample_point3_actor();

  void
  create_sample_point4_source();

  void
  update_sample_point4_source();

  void
  create_sample_point4_data_mapper();

  void
  create_sample_point4_actor();

  void
  calculate_n();

  void
  update_p1();

  void
  update_p2();

  void
  update_p3();

  void
  update_p4();

};

#endif //MMPPT_TOY_QT_VTK_EX005_SAMPLE_PLANE_HPP_

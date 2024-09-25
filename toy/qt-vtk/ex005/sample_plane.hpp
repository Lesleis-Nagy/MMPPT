//
// Created by Lesleis Nagy on 17/09/2024.
//

#ifndef MMPPT_TOY_QT_VTK_EX005_SAMPLE_PLANE_HPP_
#define MMPPT_TOY_QT_VTK_EX005_SAMPLE_PLANE_HPP_

#include <cmath>

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

  [[nodiscard]] int point_resolution_theta() const;

  [[nodiscard]] int point_resolution_phi() const;

  [[nodiscard]] const lcgl::Vector3D<double> & target() const;

  [[nodiscard]] const lcgl::Vector3D<double> & pc() const;

  [[nodiscard]] const lcgl::Vector3D<double> & p1() const;

  [[nodiscard]] const lcgl::Vector3D<double> & p2() const;

  [[nodiscard]] const lcgl::Vector3D<double> & p3() const;

  [[nodiscard]] const lcgl::Vector3D<double> & p4() const;

  [[nodiscard]] const lcgl::Vector3D<double> & n() const;

  [[nodiscard]] const lcgl::Vector3D<double> & t_theta() const;

  [[nodiscard]] const lcgl::Vector3D<double> & t_phi() const;

  [[nodiscard]] double width() const;

  [[nodiscard]] double height() const;

  [[nodiscard]] double theta() const;

  [[nodiscard]] double phi() const;

  [[nodiscard]] double gamma() const;

  [[nodiscard]] double r() const;

  [[nodiscard]] bool have_sample_point1_vtk_objects() const;

  [[nodiscard]] bool have_sample_point2_vtk_objects() const;

  [[nodiscard]] bool have_sample_point3_vtk_objects() const;

  [[nodiscard]] bool have_sample_point4_vtk_objects() const;


  //-------------------------------------------------------------------------//
  //- Public setters                                                        -//
  //-------------------------------------------------------------------------//

  void update_sample_points();

  void scale_multiplier(double value);

  void point_resolution_theta(int value);

  void point_resolution_phi(int value);

  void target(lcgl::Vector3D<double> value);

  void width(double value);

  void height(double value);

  void theta(double value);

  void phi(double value);

  void gamma(double value);

  void r(double value);

  //-------------------------------------------------------------------------//
  // Public utility functions
  //-------------------------------------------------------------------------//

  void
  add_actors_to_renderer(vtkSmartPointer<vtkRenderer> &renderer) const;

  void
  remove_actors_from_renderer(vtkSmartPointer<vtkRenderer> &renderer) const;

 private:

  double _length_scale;
  double _scale_multiplier;
  int _point_resolution_theta;
  int _point_resolution_phi;

  lcgl::Vector3D<double> _target;
  lcgl::Vector3D<double> _n;
  lcgl::Vector3D<double> _t_theta;
  lcgl::Vector3D<double> _t_phi;
  double _theta;  // polar angle
  double _phi;    // azimuthal angle
  double _gamma;  // orientation angle
  double _r;      // distance from target
  double _width;  // plane width
  double _height; // plane height

  lcgl::Vector3D<double> _pc;
  lcgl::Vector3D<double> _p1;
  lcgl::Vector3D<double> _p2;
  lcgl::Vector3D<double> _p3;
  lcgl::Vector3D<double> _p4;

  lcgl::Matrix3x3<double> _rot_matrix;

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
  update_sample_point1_actor();





  void
  create_sample_point2_source();

  void
  update_sample_point2_source();

  void
  create_sample_point2_data_mapper();

  void
  create_sample_point2_actor();

  void
  update_sample_point2_actor();





  void
  create_sample_point3_source();

  void
  update_sample_point3_source();

  void
  create_sample_point3_data_mapper();

  void
  create_sample_point3_actor();

  void
  update_sample_point3_actor();






  void
  create_sample_point4_source();

  void
  update_sample_point4_source();

  void
  create_sample_point4_data_mapper();

  void
  create_sample_point4_actor();

  void
  update_sample_point4_actor();

  const lcgl::Vector3D<double> &
  update_t_theta();

  const lcgl::Vector3D<double> &
  update_t_phi();

  const lcgl::Vector3D<double> &
  update_n();

  const lcgl::Vector3D<double> &
  update_pc();

  const lcgl::Vector3D<double> &
  update_p1();

  const lcgl::Vector3D<double> &
  update_p2();

  const lcgl::Vector3D<double> &
  update_p3();

  const lcgl::Vector3D<double> &
  update_p4();

  const lcgl::Matrix3x3<double> &
  update_rot_matrix();

};

#endif //MMPPT_TOY_QT_VTK_EX005_SAMPLE_PLANE_HPP_

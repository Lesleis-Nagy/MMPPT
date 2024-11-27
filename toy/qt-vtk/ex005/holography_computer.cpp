//
// Created by Lesleis Nagy on 04/11/2024.
//

#include "holography_computer.hpp"

HolographyComputer::HolographyComputer(
    const Model &model,
    size_t model_field_index,
    size_t npts,
    double delta) :
    _npts(npts),
    _delta(delta) {

  const auto &mesh = model.mesh();
  const auto &vcl = mesh.vcl();
  const auto &m = model.field_list().fields()[model_field_index].vectors();

  for (const auto &tet : mesh.til()) {

    const auto &m0 = m[tet[0]];
    const auto &m1 = m[tet[1]];
    const auto &m2 = m[tet[2]];
    const auto &m3 = m[tet[3]];

    auto mx = (m0[0] + m1[0] + m2[0] + m3[0]) / 4.0;
    auto my = (m0[1] + m1[1] + m2[1] + m3[1]) / 4.0;
    auto mz = (m0[2] + m1[2] + m2[2] + m3[2]) / 4.0;

    lcgl::Vector3D<double> m_avg = {mx, my, mz};
    //m_avg.normalise();

    lcgl::Vector3D<double> r1 = {
        vcl[tet[0]][0], vcl[tet[0]][1], vcl[tet[0]][2]
    };
    lcgl::Vector3D<double> r2 = {
        vcl[tet[1]][0], vcl[tet[1]][1], vcl[tet[1]][2]
    };
    lcgl::Vector3D<double> r3 = {
        vcl[tet[2]][0], vcl[tet[2]][1], vcl[tet[2]][2]
    };
    lcgl::Vector3D<double> r4 = {
        vcl[tet[3]][0], vcl[tet[3]][1], vcl[tet[3]][2]
    };

    _vp_func.push_back(new_uni_tet_A_fun(m_avg, r1, r2, r3, r4));

  }

}

lcgl::Vector3D<double>
HolographyComputer::transformed_vector_potential(
    const lcgl::Vector3D<double> &r0,
    const lcgl::Vector3D<double> &n,
    double t) const {

  double x = tan(t);

  lcgl::Vector3D<double> xx = r0 + x*n;
  double jacobian = 1.0 / (cos(t) * cos(t));  // sec^2(t) = 1 / cos^2(t)

  return jacobian * vector_potential(xx);

}

lcgl::Vector3D<double>
HolographyComputer::vector_potential(const lcgl::Vector3D<double> &r) const {

  static std::vector<lcgl::Vector3D<double>> tmp_vs(_vp_func.size());

  // Start time for serial reduction
  auto vp_start = std::chrono::high_resolution_clock::now();

  #pragma omp parallel for
  for (int i = 0; i < _vp_func.size(); ++i) {
    tmp_vs[i] =_vp_func[i](r);
  }

  lcgl::Vector3D<double> tmp_v = {0.0, 0.0, 0.0};

  #pragma omp parallel for reduction(+:tmp_v)
  for (size_t i = 0; i < _vp_func.size(); ++i) {
    tmp_v += tmp_vs[i];
  }

  auto vp_end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = vp_end - vp_start;
  std::cout << duration << " seconds\n";

  return tmp_v;

}

double
HolographyComputer::operator()(double x, double y, double z,
                               double nx, double ny, double nz) const {
  lcgl::Vector3D<double> r = {x, y, z};
  lcgl::Vector3D<double> n = {nx, ny, nz};

  return (*this)(r, n);
}

double
HolographyComputer::operator()(const lcgl::Vector3D<double> &r,
                               const lcgl::Vector3D<double> &n) const {

  double a = 0.0;
  double b = M_PI / 2.0 - _delta;

  double h = (b - a) / (double)_npts;

  lcgl::Vector3D<double> sum = vector_potential(r);

  for (size_t i = 0; i < _npts; ++i) {
    double t = a + (double)i * h;

    if (i % 2 == 0) {
      sum += 2.0 * transformed_vector_potential(r, n, t);
    } else {
      sum += 4.0 * transformed_vector_potential(r, n, t);
    }

    return dot((h / 3.0) * sum, n);
  }

  return dot(sum, n);

}
//
// Created by Lesleis Nagy on 23/10/2024.
//

#include "mfm_computer.hpp"

MFMComputer::MFMComputer(
    const Model &model,
    size_t model_field_index) {

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

    _vp_func.push_back(new_uni_tet_B_fun(m_avg, r1, r2, r3, r4));

  }

}

double
MFMComputer::operator()(double x,  double y,  double z,
                        double nx, double ny, double nz) const {

  lcgl::Vector3D<double> r = {x, y, z};
  lcgl::Vector3D<double> n = {nx, ny, nz};

  return (*this)(r, n);

}
double MFMComputer::operator()(const lcgl::Vector3D<double> &r,
                               const lcgl::Vector3D<double> &n) const {

  lcgl::Vector3D<double> B_field = {0.0, 0.0, 0.0};



  //#pragma omp parallel for num_threads(5), reduction (+:sum)
  for (size_t i = 0; i < _vp_func.size(); ++i) {
    B_field += _vp_func[i](r);
  }



  return dot(B_field, n);

}

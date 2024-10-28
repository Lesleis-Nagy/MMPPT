/**
 * @file fabbri.hpp
 * @author L. Nagy
 * @brief Functions.
 */

#ifndef LIBFABBRI_FABBRI_HPP
#define LIBFABBRI_FABBRI_HPP

#include <array>
#include <functional>

#include "lcgl.hpp"

/**
 * An object that represents a function to map positions to scalars.
 */
template<typename T>
using PosToScalarMap = std::function<T(const lcgl::Vector3D<T> &)>;

/**
 * An object that represents a function to map positions to vectors.
 */
template<typename T>
using PosToVectorMap = std::function<lcgl::Vector3D<T>(const lcgl::Vector3D<T> &)>;

/**
 * An object that represents a mapping from a magnetisation, position pair to a
 * scalar.
 */
template<typename T>
using MagPosToScalarMap = std::function<T(const lcgl::Vector3D<T> &,
                                          const lcgl::Vector3D<T> &)>;

/**
 * An object that represents a mapping from a magnetisation, position pair to a
 * vector.
 */
template<typename T>
using MagPosToVectorMap =
    std::function<
        lcgl::Vector3D<T>(
            const lcgl::Vector3D<T> &,
            const lcgl::Vector3D<T> &
        )
    >;

/**
 * Return the Jacobian of the linear variation of four vectors V1, V2, V3 & V4
 * defined on the corners of a tetrahedron with coordinates r1, r2, r3 & r4
 * respectively.
 *
 * @tparam T the underlying data type for the calculation - usually 'double'.
 *           or 'mpreal'.
 * @param r1 the first vertex of the tetrahedron.
 * @param r2 the second vertex of the tetrahedron.
 * @param r3 the third vertex of the tetrahedron.
 * @param r4 the fourth vertex of the tetrahedron.
 * @param V1 the field vector at the first vertex of the tetrahedron.
 * @param V2 the field vector at the second vertex of the tetrahedron.
 * @param V3 the field vector at the third vertex of the tetrahedron.
 * @param V4 the field vector at the fourth vertex of the tetrahedron.
 *
 * @return the Jacobian of the linear variation of four vectors.
 */
template<typename T>
lcgl::Matrix3x3<T>
DV(const lcgl::Vector3D<T> &r1, const lcgl::Vector3D<T> &r2,
   const lcgl::Vector3D<T> &r3, const lcgl::Vector3D<T> &r4,
   const lcgl::Vector3D<T> &V1, const lcgl::Vector3D<T> &V2,
   const lcgl::Vector3D<T> &V3, const lcgl::Vector3D<T> &V4) {

  using Mat3x3 = lcgl::Matrix3x3<T>;
  using Mat4x4 = lcgl::Matrix4x4<T>;

  Mat4x4 tetra = {{1.0, 1.0, 1.0, 1.0},
                  {r1.x(), r2.x(), r3.x(), r4.x()},
                  {r1.y(), r2.y(), r3.y(), r4.y()},
                  {r1.z(), r2.z(), r3.z(), r4.z()}};

  T signed_vol = -1.0 * det(tetra);

  // [c]olumn 1 [s]um <1-4>

  Mat3x3 c1s1 = {{1.0, 1.0, 1.0},
                 {r2.y(), r3.y(), r4.y()},
                 {r2.z(), r3.z(), r4.z()}};

  Mat3x3 c1s2 = {{1.0, 1.0, 1.0},
                 {r1.y(), r3.y(), r4.y()},
                 {r1.z(), r3.z(), r4.z()}};

  Mat3x3 c1s3 = {{1.0, 1.0, 1.0},
                 {r1.y(), r2.y(), r4.y()},
                 {r1.z(), r2.z(), r4.z()}};

  Mat3x3 c1s4 = {{1.0, 1.0, 1.0},
                 {r1.y(), r2.y(), r3.y()},
                 {r1.z(), r2.z(), r3.z()}};

  // [c]olumn 2 [s]um <1-4>

  Mat3x3 c2s1 = {{1.0, 1.0, 1.0},
                 {r2.x(), r3.x(), r4.x()},
                 {r2.z(), r3.z(), r4.z()}};

  Mat3x3 c2s2 = {{1.0, 1.0, 1.0},
                 {r1.x(), r3.x(), r4.x()},
                 {r1.z(), r3.z(), r4.z()}};

  Mat3x3 c2s3 = {{1.0, 1.0, 1.0},
                 {r1.x(), r2.x(), r4.x()},
                 {r1.z(), r2.z(), r4.z()}};

  Mat3x3 c2s4 = {{1.0, 1.0, 1.0},
                 {r1.x(), r2.x(), r3.x()},
                 {r1.z(), r2.z(), r3.z()}};

  // [e]dge [c]olumn 2 [s]um <1-4>

  Mat3x3 c3s1 = {{1.0, 1.0, 1.0},
                 {r2.x(), r3.x(), r4.x()},
                 {r2.y(), r3.y(), r4.y()}};

  Mat3x3 c3s2 = {{1.0, 1.0, 1.0},
                 {r1.x(), r3.x(), r4.x()},
                 {r1.y(), r3.y(), r4.y()}};

  Mat3x3 c3s3 = {{1.0, 1.0, 1.0},
                 {r1.x(), r2.x(), r4.x()},
                 {r1.y(), r2.y(), r4.y()}};

  Mat3x3 c3s4 = {{1.0, 1.0, 1.0},
                 {r1.x(), r2.x(), r3.x()},
                 {r1.y(), r2.y(), r3.y()}};


  // [d]eterminant of [c]olumn 1 [s]um <1-4>

  T dc1s1 = det(c1s1);
  T dc1s2 = det(c1s2);
  T dc1s3 = det(c1s3);
  T dc1s4 = det(c1s4);

  // [d]eterminant of  [c]olumn 2 [s]um <1-4>

  T dc2s1 = det(c2s1);
  T dc2s2 = det(c2s2);
  T dc2s3 = det(c2s3);
  T dc2s4 = det(c2s4);

  // [d]eterminant of [c]olumn 3 [s]um <1-4>

  T dc3s1 = det(c3s1);
  T dc3s2 = det(c3s2);
  T dc3s3 = det(c3s3);
  T dc3s4 = det(c3s4);

  lcgl::Matrix3x3<T> DV = {
      // Row r=0
      {V1.x() * dc1s1 - V2.x() * dc1s2 + V3.x() * dc1s3 - V4.x() * dc1s4,
       -V1.x() * dc2s1 + V2.x() * dc2s2 - V3.x() * dc2s3 + V4.x() * dc2s4,
       V1.x() * dc3s1 - V2.x() * dc3s2 + V4.x() * dc3s3 - V4.x() * dc3s4},

      // Row r=1
      {V1.y() * dc1s1 - V2.y() * dc1s2 + V3.y() * dc1s3 - V4.y() * dc1s4,
       -V1.y() * dc2s1 + V2.y() * dc2s2 - V3.y() * dc2s3 + V4.y() * dc2s4,
       V1.y() * dc3s1 - V2.y() * dc3s2 + V4.y() * dc3s3 - V4.y() * dc3s4},

      // Row r=2
      {V1.z() * dc1s1 - V2.z() * dc1s2 + V3.z() * dc1s3 - V4.z() * dc1s4,
       -V1.z() * dc2s1 + V2.z() * dc2s2 - V3.z() * dc2s3 + V4.z() * dc2s4,
       V1.z() * dc3s1 - V2.z() * dc3s2 + V4.z() * dc3s3 - V4.z() * dc3s4}
  };

  return DV / signed_vol;

}

/**
 * Return the curl of the linearly varying vector field defined over a
 * tetrahedron with vertices r1, r2, r3, r4; and vector field values
 * V1(r1), V2(r2), V3(r3) & V4(r4).
 */
template<typename T>
lcgl::Vector3D<T>
curl(const lcgl::Vector3D<T> &r1, const lcgl::Vector3D<T> &r2,
     const lcgl::Vector3D<T> &r3, const lcgl::Vector3D<T> &r4,
     const lcgl::Vector3D<T> &V1, const lcgl::Vector3D<T> &V2,
     const lcgl::Vector3D<T> &V3, const lcgl::Vector3D<T> &V4) {

  using Mat3x3 = lcgl::Matrix3x3<T>;
  using Mat4x4 = lcgl::Matrix4x4<T>;

  Mat4x4 tetra = {{1.0, 1.0, 1.0, 1.0},
                  {r1.x(), r2.x(), r3.x(), r4.x()},
                  {r1.y(), r2.y(), r3.y(), r4.y()},
                  {r1.z(), r2.z(), r3.z(), r4.z()}};

  T signed_vol = -1.0 * det(tetra);

  // xy

  // Determinant of matrix x2x3x4_y2y3y4
  T x2x3x4_y2y3y4 = det(Mat3x3{{1.0, 1.0, 1.0},
                               {r2.x(), r3.x(), r4.x()},
                               {r2.y(), r3.y(), r4.y()}});

  // Determinant of matrix x1x3x4_y1y3y4
  T x1x3x4_y1y3y4 = det(Mat3x3{{1.0, 1.0, 1.0},
                               {r1.x(), r3.x(), r4.x()},
                               {r1.y(), r3.y(), r4.y()}});

  // Determinant of matrix x1x2x4_y1y2y4
  T x1x2x4_y1y2y4 = det(Mat3x3{{1.0, 1.0, 1.0},
                               {r1.x(), r2.x(), r4.x()},
                               {r1.y(), r2.y(), r4.y()}});

  // Determinant of matrix x1x2x3_y1y2y3
  T x1x2x3_y1y2y3 = det(Mat3x3{{1.0, 1.0, 1.0},
                               {r1.x(), r2.x(), r3.x()},
                               {r1.y(), r2.y(), r3.y()}});

  // yz

  // Determinant of matrix y2y3y4_z2z3z4
  T y2y3y4_z2z3z4 = det(Mat3x3{{1.0, 1.0, 1.0},
                               {r2.y(), r3.y(), r4.y()},
                               {r2.z(), r3.z(), r4.z()}});

  // Determinant of matrix y1y3y4_z1z3z4
  T y1y3y4_z1z3z4 = det(Mat3x3{{1.0, 1.0, 1.0},
                               {r1.y(), r3.y(), r4.y()},
                               {r1.z(), r3.z(), r4.z()}});

  // Determinant of matrix y1y2y4_z1z2z4
  T y1y2y4_z1z2z4 = det(Mat3x3{{1.0, 1.0, 1.0},
                               {r1.y(), r2.y(), r4.y()},
                               {r1.z(), r2.z(), r4.z()}});

  // Determinant of matrix y1y2y3_z1z2z3
  T y1y2y3_z1z2z3 = det(Mat3x3{{1.0, 1.0, 1.0},
                               {r1.y(), r2.y(), r3.y()},
                               {r1.z(), r2.z(), r3.z()}});

  // xz

  // Determinant of matrix x2x3x4_z2z3z4
  T x2x3x4_z2z3z4 = det(Mat3x3{{1.0, 1.0, 1.0},
                               {r2.x(), r3.x(), r4.x()},
                               {r2.z(), r3.z(), r4.z()}});

  // Determinant of matrix x1x3x4_z1z3z4
  T x1x3x4_z1z3z4 = det(Mat3x3{{1.0, 1.0, 1.0},
                               {r1.x(), r3.x(), r4.x()},
                               {r1.z(), r3.z(), r4.z()}});

  // Determinant of matrix x1x2x4_z1z2z4
  T x1x2x4_z1z2z4 = det(Mat3x3{{1.0, 1.0, 1.0},
                               {r1.x(), r2.x(), r4.x()},
                               {r1.z(), r2.z(), r4.z()}});

  // Determinant of matrix x1x2x3_z1z2z3
  T x1x2x3_z1z2z3 = det(Mat3x3{{1.0, 1.0, 1.0},
                               {r1.x(), r2.x(), r3.x()},
                               {r1.z(), r2.z(), r3.z()}});

  return {
      // Entry x.
      (-V1.y() * x2x3x4_y2y3y4
          + V2.y() * x1x3x4_y1y3y4
          - V3.y() * x1x2x4_y1y2y4
          + V4.y() * x1x2x3_y1y2y3
          - V1.z() * x2x3x4_z2z3z4
          + V2.z() * x1x3x4_z1z3z4
          - V3.z() * x1x2x4_z1z2z4
          + V4.z() * x1x2x3_z1z2z3) / signed_vol,

      // Entry y
      (V1.x() * x2x3x4_y2y3y4
          - V2.x() * x1x3x4_y1y3y4
          + V3.x() * x1x2x4_y1y2y4
          - V4.x() * x1x2x3_y1y2y3
          - V1.z() * y2y3y4_z2z3z4
          + V2.z() * y1y3y4_z1z3z4
          - V3.z() * y1y2y4_z1z2z4
          + V4.z() * y1y2y3_z1z2z3) / signed_vol,

      // Entry z
      (V1.x() * x2x3x4_z2z3z4
          - V2.x() * x1x3x4_z1z3z4
          + V3.x() * x1x2x4_z1z2z4
          - V4.x() * x1x2x3_z1z2z3
          + V1.y() * y2y3y4_z2z3z4
          - V2.y() * y1y3y4_z1z3z4
          + V3.y() * y1y2y4_z1z2z4
          - V4.y() * y1y2y3_z1z2z3) / signed_vol
  };

}

/**
 * Return a function that will calculate the solid angle \f$\Omega\f$ subtended
 * by the triangle with vertices \f$r_1\f$, \f$r_2\f$ & \f$r_3\f$ as a function
 * of space - see eq. (21) in Fabbri, 2008.
 *
 * @tparam T the underlying data type for the calculation - usually 'double'
 *           or 'mpreal'.
 * @param r1 the first vertex of the subtending triangle.
 * @param r2 the second vertex of the subtending triangle.
 * @param r3 the third vertex of the subtending triangle.
 *
 * @return the function \f${\Omega}\left(\vec{r}\right)\f$ (with \f$\vec{r}\f$
 *         being a Vector3D<T> object) that calculates the solid angle at the
 *         input point subtended by the triangle with vertices \f$r_1\f$,
 *         \f$r_2\f$ & \f$r_3\f$.
 */
template<typename T>
PosToScalarMap<T>
new_omega_tri_fun(const lcgl::Vector3D<T> &r1,
                  const lcgl::Vector3D<T> &r2,
                  const lcgl::Vector3D<T> &r3) {

  return [r1, r2, r3](const lcgl::Vector3D<T> &r) {

    lcgl::Vector3D<T> v1 = (r1 - r);
    lcgl::Vector3D<T> v2 = (r2 - r);
    lcgl::Vector3D<T> v3 = (r3 - r);

    T d1 = norm(v1);
    T d2 = norm(v2);
    T d3 = norm(v3);

    return 2.0 * atan2(
        dot(v1, cross(v2, v3)),
        d1 * d2 * d3 + d3 * dot(v1, v2) + d2 * dot(v1, v3) + d1 * dot(v2, v3)
    );

  };

}

/**
 * Return a function that will calculate the edge potential function
 * \f$w_\mathrm{e}\f$ eq. (18) for a line segment with endpoints
 * \f$r_1\f$ & \f$r_2\f$ (Fabbri, 2008).
 * @tparam T the underlying data type for the calculation - usually 'double'
 *           or 'mpreal'.
 * @param r1 the first point of a line segment.
 * @param r2 the second point of a line segment.
 * @return a function that will calculate \f$w_\mathrm{e}\f$.
 */
template<typename T>
PosToScalarMap<T>
new_we_fun(const lcgl::Vector3D<T> &r1, const lcgl::Vector3D<T> &r2) {

  T d_const = norm(r2 - r1);

  return [r1, r2, d_const](const lcgl::Vector3D<T> &r) {

    T d1 = norm(r1 - r);
    T d2 = norm(r2 - r);

    return log((d2 + d1 + d_const) / (d2 + d1 - d_const));

  };

}

/**
 * Return a function that will calculate \f$\lambda_\mathrm{e}\f$ in eq. (22)
 * for a line segment with endpoints \f$r_1\f$ and \f$r_2\f$ (Fabbri, 2009).
 * @tparam T the underlying data type for the calculation - usually 'double'
 *           or 'mpreal'.
 * @param r1 the first point of a line segment.
 * @param r2 the second point of a line segment.
 * @return a function that will calculate \f$\lambda_\mathrm{e}\f$.
 */
template<typename T>
PosToScalarMap<T>
new_lambda_e_fun(const lcgl::Vector3D<T> &r1, const lcgl::Vector3D<T> &r2) {

  using std::function;

  function we = new_we_fun(r1, r2);
  lcgl::Vector3D<T> ue = normalised(r2 - r1);
  lcgl::Vector3D<T> re = edge_center(r1, r2);

  return [r1, r2, we, ue, re](const lcgl::Vector3D<T> &r) {

    return 0.5 * norm(r2 - r) * dot((r2 - r), ue) +
        0.5 * norm(r1 - r) * dot((r1 - r), ue) +
        0.5 * norm_squared(cross((r - re), ue)) * we(r);

  };

}

/**
 * Return a function that will calculate \f$\nabla \lambda_\mathrm{e}\f$ in eq.
 * (29) for a line segment with endpoints \f$r_1\f$ and \f$r_2\f$
 * (Fabbri, 2009).
 * @tparam T the underlying data type for the calculation - usually 'double'
 *           or 'mpreal'.
 * @param r1 the first point of a line segment.
 * @param r2 the second point of a line segment.
 * @return a function that will calculate \f$\nabla \lambda_\mathrm{e}\f$.
 */
template<typename T>
PosToVectorMap<T>
new_D_lambda_e_fun(const lcgl::Vector3D<T> &r1, const lcgl::Vector3D<T> &r2) {

  using std::function;

  function we = new_we_fun(r1, r2);
  lcgl::Vector3D<T> ue = normalised(r2 - r1);
  lcgl::Vector3D<T> re = edge_center(r1, r2);

  return [r1, r2, we, ue, re](const lcgl::Vector3D<T> &r) {

    return (norm(r1 - r) - norm(r2 - r)) * ue +
        cross(ue, cross((r - re), ue)) * we(r);

  };

}

/**
 * Return a function that will calculate
 * \f$\frac{\partial \lambda_\mathrm{e}}{\partial m}\f$ for a line segment with
 * endpoints \f$r_1\f$ and \f$r_2\f$ (Fabbri, 2008).
 * @tparam T the underlying data type for the calculation - usually 'double'
 *           or 'mpreal'.
 * @param r1 the first point of a line segment.
 * @param r2 the second point of a line segment.
 * @return the function
 *         \f$\frac{\partial \lambda_\mathrm{e}}{\partial m}\left(\frac{\partial W_\mathrm{f}}{\partial m}\right)\f$
 *         with first input magnetisation and second input position.
 */
template<typename T>
MagPosToScalarMap<T>
new_d_lambda_e_by_dm_fun(const lcgl::Vector3D<T> &r1,
                         const lcgl::Vector3D<T> &r2) {

  using std::function;

  function D_lambda_e = new_D_lambda_e_fun(r1, r2);

  return [D_lambda_e](const lcgl::Vector3D<T> &m, const lcgl::Vector3D<T> &r) {

    return dot(m, D_lambda_e(r));

  };

}

/**
 * Return a function that will calculate the face-potential function
 * \f$W_\mathrm{f}\f$ eq. (17) for a triangular polyhedron (Fabbri, 2008) with
 * endpoints \f$r_1\f$, \f$r_2\f$ & \f$r_3\f$ and counter-clockwise winding
 * \f$r_1 \rightarrow r2\f$, \f$r_2 \rightarrow r_3\f$ & \f$r3 \rightarrow r1\f$.
 * @tparam T the underlying data type for the calculation - usually 'double'
 *           or 'mpreal'.
 * @param r1 the first point of a triangle segment.
 * @param r2 the second point of a triangle segment.
 * @param r3 the third point of a triangle.
 * @return a function that will calculate \f$W_\mathrm{f}\f$.
 */
template<typename T>
PosToScalarMap<T>
new_Wf_tri_fun(const lcgl::Vector3D<T> &r1,
               const lcgl::Vector3D<T> &r2,
               const lcgl::Vector3D<T> &r3) {

  using std::array;
  using std::function;

  // Face normal.
  lcgl::Vector3D<T> nf = lcgl::triangle_normal(r1, r2, r3);

  // Arbitrary point on the face.
  lcgl::Vector3D<T> rf = triangle_center(r1, r2, r3);

  // Solid angle subtended by this triangle.
  function<T(const lcgl::Vector3D<T> &)> omega = new_omega_tri_fun(r1, r2, r3);

  // we functions.
  array<function<T(const lcgl::Vector3D<T> &)>, 3> we;

  we[0] = new_we_fun(r1, r2);
  we[1] = new_we_fun(r2, r3);
  we[2] = new_we_fun(r3, r1);

  array<lcgl::Vector3D<T>, 3> re;
  re[0] = edge_center(r1, r2);
  re[1] = edge_center(r2, r3);
  re[2] = edge_center(r3, r1);

  array<lcgl::Vector3D<T>, 3> ue;
  ue[0] = edge_orientation(r1, r2);
  ue[1] = edge_orientation(r2, r3);
  ue[2] = edge_orientation(r3, r1);

  return [nf, rf, omega, re, ue, we](const lcgl::Vector3D<T> &r) {

    return dot(cross(nf, re[0] - r), ue[0]) * we[0](r) +
        dot(cross(nf, re[1] - r), ue[1]) * we[1](r) +
        dot(cross(nf, re[2] - r), ue[2]) * we[2](r) -
        dot(rf - r, nf) * omega(r);

  };

}

/**
 * Return a function that will calculate the gradient of Fabbri's face potential
 * function \f$\Delta W_\mathrm{f}\f$ eq. (25) for a triangular polygon
 * (Fabbri, 2008) with endpoints \f$r_1\f$, \f$r_2\f$ & \f$r_3\f$ and winding
 * \f$r_1 \rightarrow r2\f$, \f$r_2 \rightarrow r_3\f$ & \f$r3 \rightarrow r1\f$.
 * @tparam T the underlying data type for the calculation - usually 'double'
 *           or 'mpreal'.
 * @param r1 the first point of a triangle segment.
 * @param r2 the second point of a triangle segment.
 * @param r3 the third point of a triangle.
 * @return a function that will calculate \f$\Delta W_\mathrm{f}\f$.
 */
template<typename T>
PosToVectorMap<T>
new_DWf_tri_fun(const lcgl::Vector3D<T> &r1,
                const lcgl::Vector3D<T> &r2,
                const lcgl::Vector3D<T> &r3) {

  using std::array;
  using std::function;

  // Face normal.
  lcgl::Vector3D<T> nf = lcgl::triangle_normal(r1, r2, r3);

  // Solid angle subtended by this triangle.
  function<T(const lcgl::Vector3D<T> &)> omega = new_omega_tri_fun(r1, r2, r3);

  // we functions.
  array<function<T(const lcgl::Vector3D<T> &)>, 3> we;

  we[0] = new_we_fun(r1, r2);
  we[1] = new_we_fun(r2, r3);
  we[2] = new_we_fun(r3, r1);

  array<lcgl::Vector3D<T>, 3> ue;
  ue[0] = edge_orientation(r1, r2);
  ue[1] = edge_orientation(r2, r3);
  ue[2] = edge_orientation(r3, r1);

  return [nf, omega, ue, we](const lcgl::Vector3D<T> &r) {

    return cross(nf, ue[0]) * we[0](r) + cross(nf, ue[1]) * we[1](r) +
        cross(nf, ue[2]) * we[2](r) + nf * omega(r);

  };

}

/**
 * Return a function that will calculate
 * \f$\frac{\partial W_\mathrm{f}}{\partial m}\f$ for a triangle with
 * endpoints \f$r_1\f$, \f$r_2\f$ & \f$r_3\f$ (Fabbri, 2009).
 * @tparam T the underlying data type for the calculation - usually 'double'
 *           or 'mpreal'.
 * @param r1 the first point of a line segment.
 * @param r2 the second point of a line segment.
 * @param r3 the third point of a line segment.
 * @return a function that will calculate
 *         \f$\frac{\partial W_\mathrm{f}}{\partial m}\left(\vec{m}, \vec{r} \right)\f$,
 *         with first input magnetisation and second input position.
 */
template<typename T>
MagPosToScalarMap<T>
new_d_Wf_by_dm_tri_fun(const lcgl::Vector3D<T> &r1,
                       const lcgl::Vector3D<T> &r2,
                       const lcgl::Vector3D<T> &r3) {

  using std::function;

  function DWf = new_DWf_tri_fun(r1, r2, r3);

  return [DWf](const lcgl::Vector3D<T> &m, const lcgl::Vector3D<T> &r) {

    return dot(m, DWf(r));

  };

}

/**
 * Return a function that will calculate \f$\Lambda_f\f$ eq. (21), for a
 * triangular polygon (Fabbri, 2009) with endpoints
 * \f$r_1\f$, \f$r_2\f$ & \f$r_3\f$ and winding
 * \f$r_1 \rightarrow r2\f$, \f$r_2 \rightarrow r_3\f$ and
 * \f$r3 \rightarrow r1\f$.
 * @tparam T the underlying data type for the calculation - usually 'double'
 *           or 'mpreal'.
 * @param r1 the first point of a triangle segment.
 * @param r2 the second point of a triangle segment.
 * @param r3 the third point of a triangle.
 * @return a function that will calculate \f$\Lambda_f\f$.
 */
template<typename T>
PosToScalarMap<T>
new_Lambda_f_fun(const lcgl::Vector3D<T> &r1,
                 const lcgl::Vector3D<T> &r2,
                 const lcgl::Vector3D<T> &r3) {

  using std::array;
  using std::function;

  // Face normal.
  lcgl::Vector3D<T> nf = lcgl::triangle_normal(r1, r2, r3);

  // Midpoints.
  array<lcgl::Vector3D<T>, 3> re;
  re[0] = edge_center(r1, r2);
  re[1] = edge_center(r2, r3);
  re[2] = edge_center(r3, r1);

  // Edge orientation vectors.
  array<lcgl::Vector3D<T>, 3> ue;
  ue[0] = normalised(r2 - r1);
  ue[1] = normalised(r3 - r2);
  ue[2] = normalised(r1 - r3);

  // Edge lambda_e functions.
  array<function<T(const lcgl::Vector3D<T> &)>, 3> lambda_e;
  lambda_e[0] = new_lambda_e_fun(r1, r2);
  lambda_e[1] = new_lambda_e_fun(r2, r3);
  lambda_e[2] = new_lambda_e_fun(r3, r1);

  // Triangle centroid.
  lcgl::Vector3D<T> rf = triangle_center(r1, r2, r3);

  // Wf function
  function<T(const lcgl::Vector3D<T> &)> Wf = new_Wf_tri_fun(r1, r2, r3);

  return [nf, re, ue, lambda_e, rf, Wf](const lcgl::Vector3D<T> &r) {

    return (T{1.0} / T{3.0}) *
        (dot(cross(nf, re[0] - r), ue[0]) * lambda_e[0](r) +
            dot(cross(nf, re[1] - r), ue[1]) * lambda_e[1](r) +
            dot(cross(nf, re[2] - r), ue[2]) * lambda_e[2](r) +
            dot((rf - r), nf) * dot((rf - r), nf) * Wf(r));

  };

}

/**
 * Return a function that will calculate \f$\nabla\Lambda_f\f$ eq. (25), for a
 * triangular polygon (Fabbri, 2009) with vertices
 * \f$r_1\f$, \f$r_2\f$ & \f$r_3\f$ and winding \f$r_1 \rightarrow r2\f$,
 * \f$r_2 \rightarrow r_3\f$ and \f$r3 \rightarrow r1\f$.
 * @tparam T the underlying data type for the calculation - usually 'double'
 *           or 'mpreal'.
 * @param r1 the first point of a triangle segment.
 * @param r2 the second point of a triangle segment.
 * @param r3 the third point of a triangle.
 * @return a function that will calculate \f$\nabla\Lambda_f\f$.
 */
template<typename T>
PosToVectorMap<T>
new_DLambda_f_fun(const lcgl::Vector3D<T> &r1,
                  const lcgl::Vector3D<T> &r2,
                  const lcgl::Vector3D<T> &r3) {

  using std::array;
  using std::function;

  // Face normal.
  lcgl::Vector3D<T> nf = lcgl::triangle_normal(r1, r2, r3);

  // Edge orientation vectors.
  array<lcgl::Vector3D<T>, 3> ue;
  ue[0] = normalised(r2 - r1);
  ue[1] = normalised(r3 - r2);
  ue[2] = normalised(r1 - r3);

  // Edge lambda_e functions.
  array<function<T(const lcgl::Vector3D<T> &)>, 3> lambda_e;
  lambda_e[0] = new_lambda_e_fun(r1, r2);
  lambda_e[1] = new_lambda_e_fun(r2, r3);
  lambda_e[2] = new_lambda_e_fun(r3, r1);

  // Triangle centroid.
  lcgl::Vector3D<T> rf = triangle_center(r1, r2, r3);

  // Wf function
  function<T(const lcgl::Vector3D<T> &)> Wf = new_Wf_tri_fun(r1, r2, r3);

  return [nf, ue, lambda_e, rf, Wf](const lcgl::Vector3D<T> &r) {

    return cross(nf, ue[0]) * lambda_e[0](r) +
        cross(nf, ue[1]) * lambda_e[1](r) +
        cross(nf, ue[2]) * lambda_e[2](r) - nf * dot(rf - r, nf) * Wf(r);

  };

}

/**
 * Return a function that will calculate \f$\vec{m}\cdot\nabla\nabla\Lambda_f\f$
 * eq. (28), for a triangular polygon (Fabbri, 2009) with endpoints
 * \f$r_1\f$, \f$r_2\f$ & \f$r_3\f$ and winding \f$r_1 \rightarrow r2\f$,
 * \f$r_2 \rightarrow r_3\f$ and \f$r3 \rightarrow r1\f$.
 * @tparam T the underlying data type for the calculation - usually 'double'
 *           or 'mpreal'.
 * @param r1 the first point of a triangle segment.
 * @param r2 the second point of a triangle segment.
 * @param r3 the third point of a triangle.
 * @return a function that will calculate
 *         \f$\vec{m}\cdot\nabla\nabla\Lambda_f\f$.
 */
template<typename T>
MagPosToVectorMap<T>
new_m_dot_DDLambda_f_fun(const lcgl::Vector3D<T> &r1,
                         const lcgl::Vector3D<T> &r2,
                         const lcgl::Vector3D<T> &r3) {

  using std::array;
  using std::function;

  // Face normal.
  lcgl::Vector3D<T> nf = lcgl::triangle_normal(r1, r2, r3);

  // Edge orientation vectors.
  array<lcgl::Vector3D<T>, 3> ue;
  ue[0] = normalised(r2 - r1);
  ue[1] = normalised(r3 - r2);
  ue[2] = normalised(r1 - r3);

  // d_lambda_e_by_dm functions for each edge.
  array<function<T(const lcgl::Vector3D<T> &,
                   const lcgl::Vector3D<T> &)>, 3> d_lambda_e_by_dm;
  d_lambda_e_by_dm[0] = new_d_lambda_e_by_dm_fun(r1, r2);
  d_lambda_e_by_dm[1] = new_d_lambda_e_by_dm_fun(r2, r3);
  d_lambda_e_by_dm[2] = new_d_lambda_e_by_dm_fun(r3, r1);

  // Triangle centroid.
  lcgl::Vector3D<T> rf = triangle_center(r1, r2, r3);

  // Wf function.
  function<T(const lcgl::Vector3D<T> &)> Wf = new_Wf_tri_fun(r1, r2, r3);

  // d_Wf_dm function for the triangle.
  function<T(const lcgl::Vector3D<T> &,
             const lcgl::Vector3D<T> &)>
      d_Wf_dm = new_d_Wf_by_dm_tri_fun(r1, r2,
                                       r3);

  return [nf, ue, d_lambda_e_by_dm, rf, Wf, d_Wf_dm](const lcgl::Vector3D<T> &m,
                                                     const lcgl::Vector3D<T> &r) {

    return cross(nf, ue[0]) * d_lambda_e_by_dm[0](m, r) +
        cross(nf, ue[1]) * d_lambda_e_by_dm[1](m, r) +
        cross(nf, ue[2]) * d_lambda_e_by_dm[2](m, r) +
        nf * dot(m, nf) * Wf(r) - nf * dot(rf - r, nf) * d_Wf_dm(m, r);

  };

}

/**
 * Return a function that will calculate the magnetic scalar potential
 * \f$\phi\f$ for a uniformly magnetised tetrahedron using eq. (11)
 * (Fabbri, 2008). The input tetrahedron assumes the following winding for each
 * triangular face:
 * - \f$r_1 \rightarrow r_2 \rightarrow r_3\f$,
 * - \f$r_1 \rightarrow r_3 \rightarrow r_4\f$,
 * - \f$r_1 \rightarrow r_4 \rightarrow r_2\f$,
 * - \f$r_2 \rightarrow r_4 \rightarrow r_3\f$.
 * @tparam T the underlying data type for the calculation - usually 'double'
 *           or 'mpreal'.
 * @param M the (uniform) magnetisation of the tetrahedron.
 * @param r1 the first vertex of the tetrahedron.
 * @param r2 the second vertex of the tetrahedron.
 * @param r3 the third vertex of the tetrahedron.
 * @param r4 the fourth vertex of the tetrahedron.
 * @return a function that will calculate the scalar potential at any point in
 *         space due to the uniformly magnetised tetrahedron defined by the
 *         inputs.
 */
template<typename T>
PosToScalarMap<T>
new_uni_tet_phi_fun(const lcgl::Vector3D<T> &M,
                    const lcgl::Vector3D<T> &r1,
                    const lcgl::Vector3D<T> &r2,
                    const lcgl::Vector3D<T> &r3,
                    const lcgl::Vector3D<T> &r4) {

  using std::array;
  using std::function;

  // Face normals.
  array<lcgl::Vector3D<T>, 4> nf;
  nf[0] = lcgl::triangle_normal(r1, r2, r3);
  nf[1] = lcgl::triangle_normal(r1, r3, r4);
  nf[2] = lcgl::triangle_normal(r1, r4, r2);
  nf[3] = lcgl::triangle_normal(r2, r4, r3);

  // Wf functions.
  array<function<T(const lcgl::Vector3D<T> &)>, 4> Wf;
  Wf[0] = new_Wf_tri_fun(r1, r2, r3);
  Wf[1] = new_Wf_tri_fun(r1, r3, r4);
  Wf[2] = new_Wf_tri_fun(r1, r4, r2);
  Wf[3] = new_Wf_tri_fun(r2, r4, r3);

  return [M, nf, Wf](const lcgl::Vector3D<T> &r) {
    return dot(M, nf[0]) * Wf[0](r) + dot(M, nf[1]) * Wf[1](r) +
        dot(M, nf[2]) * Wf[2](r) + dot(M, nf[3]) * Wf[3](r);
  };

}

/**
 * Return a function that will calculate the magnetic vector potential
 * \f$\mathbf{A}\f$ for a uniformly magnetised tetrahedron using eq. (11)
 * (Fabbri, 2008). The input tetrahedron assumes the following winding for each
 * triangular face:
 * - \f$r_1 \rightarrow r_2 \rightarrow r_3\f$,
 * - \f$r_1 \rightarrow r_3 \rightarrow r_4\f$,
 * - \f$r_1 \rightarrow r_4 \rightarrow r_2\f$,
 * - \f$r_2 \rightarrow r_4 \rightarrow r_3\f$.
 *
 * @tparam T the underlying data type for the calculation - usually 'double'
 *           or 'mpreal'.
 * @param M the (uniform) magnetisation of the tetrahedron.
 * @param r1 the first vertex of the tetrahedron.
 * @param r2 the second vertex of the tetrahedron.
 * @param r3 the third vertex of the tetrahedron.
 * @param r4 the fourth vertex of the tetrahedron.
 * @return a function that will calculate the vector potential at any point in
 *         space due to the uniformly magnetised tetrahedron defined by the
 *         inputs.
 */
template<typename T>
PosToVectorMap<T>
new_uni_tet_A_fun(const lcgl::Vector3D<T> &M, const lcgl::Vector3D<T> &r1,
                  const lcgl::Vector3D<T> &r2, const lcgl::Vector3D<T> &r3,
                  const lcgl::Vector3D<T> &r4) {

  using std::array;
  using std::function;

  // Face normals.
  array<lcgl::Vector3D<T>, 4> nf;
  nf[0] = lcgl::triangle_normal(r1, r2, r3);
  nf[1] = lcgl::triangle_normal(r1, r3, r4);
  nf[2] = lcgl::triangle_normal(r1, r4, r2);
  nf[3] = lcgl::triangle_normal(r2, r4, r3);

  // Wf functions.
  array<function<T(const lcgl::Vector3D<T> &)>, 4> Wf;
  Wf[0] = new_Wf_tri_fun(r1, r2, r3);
  Wf[1] = new_Wf_tri_fun(r1, r3, r4);
  Wf[2] = new_Wf_tri_fun(r1, r4, r2);
  Wf[3] = new_Wf_tri_fun(r2, r4, r3);

  return [M, nf, Wf](const lcgl::Vector3D<T> &r) {
    return cross(M, nf[0]) * Wf[0](r) + cross(M, nf[1]) * Wf[1](r) +
        cross(M, nf[2]) * Wf[2](r) + cross(M, nf[3]) * Wf[3](r);
  };

}

/**
 * Return a function that will calculate the magnetic induction \f$\mathbf{B}\f$
 * for a uniformly magnetised tetrahedron using eq. (12) (Fabbri, 2008). The
 * input tetrahedron assumes the following winding for each triangular face:
 * - \f$r_1 \rightarrow r_2 \rightarrow r_3\f$,
 * - \f$r_1 \rightarrow r_3 \rightarrow r_4\f$,
 * - \f$r_1 \rightarrow r_4 \rightarrow r_2\f$,
 * - \f$r_2 \rightarrow r_4 \rightarrow r_3\f$.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param M the (uniform) magnetisation of the tetrahedron.
 * @param r1 the first vertex of the tetrahedron.
 * @param r2 the second vertex of the tetrahedron.
 * @param r3 the third vertex of the tetrahedron.
 * @param r4 the fourth vertex of the tetrahedron.
 * @return a function that will calculate the magnetic induction at any point in
 *         space due to the uniformly magnetised tetrahedron defined by the
 *         inputs.
 */
template<typename T>
PosToVectorMap<T>
new_uni_tet_B_fun(const lcgl::Vector3D<T> &M, const lcgl::Vector3D<T> &r1,
                  const lcgl::Vector3D<T> &r2, const lcgl::Vector3D<T> &r3,
                  const lcgl::Vector3D<T> &r4) {

  using std::array;
  using std::function;

  // Face normals.
  array<lcgl::Vector3D<T>, 4> nf;
  nf[0] = lcgl::triangle_normal(r1, r2, r3);
  nf[1] = lcgl::triangle_normal(r1, r3, r4);
  nf[2] = lcgl::triangle_normal(r1, r4, r2);
  nf[3] = lcgl::triangle_normal(r2, r4, r3);

  // DWf functions.
  array<function<lcgl::Vector3D<T>(const lcgl::Vector3D<T> &)>, 4> DWf;
  DWf[0] = new_DWf_tri_fun(r1, r2, r3);
  DWf[1] = new_DWf_tri_fun(r1, r3, r4);
  DWf[2] = new_DWf_tri_fun(r1, r4, r2);
  DWf[3] = new_DWf_tri_fun(r2, r4, r3);

  return [M, nf, DWf](const lcgl::Vector3D<T> &r) {
    return cross(cross(M, nf[0]), DWf[0](r)) +
        cross(cross(M, nf[1]), DWf[1](r)) +
        cross(cross(M, nf[2]), DWf[2](r)) +
        cross(cross(M, nf[3]), DWf[3](r));
  };

}

/**
 * Return a function that will calculate the magnetic scalar potential
 * \f$\mathbb{A}\f$ for a linearly magnetised tetrahedron using
 * (15) from (fabbri, 2009). The input tetrahedron assumes the following winding
 * for each triangular face:
 * - \f$r_1 \rightarrow r_2 \rightarrow r_3\f$,
 * - \f$r_1 \rightarrow r_3 \rightarrow r_4\f$,
 * - \f$r_1 \rightarrow r_4 \rightarrow r_2\f$,
 * - \f$r_2 \rightarrow r_4 \rightarrow r_3\f$.
 *
 * @tparam T the underlying data type for the calculation - usually 'double'
 *           or 'mpreal'.
 * @param M the (uniform) magnetisation of the tetrahedron.
 * @param r1 the first vertex of the tetrahedron.
 * @param r2 the second vertex of the tetrahedron.
 * @param r3 the third vertex of the tetrahedron.
 * @param r4 the fourth vertex of the tetrahedron.
 * @return a function that will calculate the vector potential at any point in
 *         space due to the uniformly magnetised tetrahedron defined by the
 *         inputs.
 */
template<typename T>
PosToVectorMap<T>
new_lin_tet_A_fun(const lcgl::Vector3D<T> &M1, const lcgl::Vector3D<T> &r1,
                  const lcgl::Vector3D<T> &M2, const lcgl::Vector3D<T> &r2,
                  const lcgl::Vector3D<T> &M3, const lcgl::Vector3D<T> &r3,
                  const lcgl::Vector3D<T> &M4, const lcgl::Vector3D<T> &r4) {

  using std::array;
  using std::function;

  // Face normals.
  array<lcgl::Vector3D<T>, 4> nf;
  nf[0] = lcgl::triangle_normal(r1, r2, r3);
  nf[1] = lcgl::triangle_normal(r1, r3, r4);
  nf[2] = lcgl::triangle_normal(r1, r4, r2);
  nf[3] = lcgl::triangle_normal(r2, r4, r3);

  // Wf functions.
  array<function<T(const lcgl::Vector3D<T> &)>, 4> Wf;
  Wf[0] = new_Wf_tri_fun(r1, r2, r3);
  Wf[1] = new_Wf_tri_fun(r1, r3, r4);
  Wf[2] = new_Wf_tri_fun(r1, r4, r2);
  Wf[3] = new_Wf_tri_fun(r2, r4, r3);

  // TODO: Finish implementation.

}

/*
 * References
 * 1.Fabbri, M.
 * Magnetic Flux Density and Vector Potential of Uniform Polyhedral Sources.
 * IEEE T. Magn. 44, 32–36 (2008).
 *
 * 2.Fabbri, M.
 * Magnetic flux density and vector potential of linear polyhedral sources.
 * Compel - Int. J .Comput. Math. Electr. Electron. Eng. 28, 1688–1700 (2009).
 */

#endif //LIBFABBRI_FABBRI_HPP

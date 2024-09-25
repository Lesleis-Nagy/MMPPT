//
// Created by Lesleis Nagy on 17/09/2024.
//

#ifndef LCGL__LCGL_HPP_
#define LCGL__LCGL_HPP_

#include <array>
#include <cuchar>
#include <iostream>

namespace lcgl {

//---------------------------------------------------------------------------//
// 3 dimensional vector.                                                     //
//---------------------------------------------------------------------------//

/**
 * An implementation of a three dimensional cartesian vector.
 * @tparam T the underlying data type for the calculation - usually ‘double’ or
 *           ‘mpreal’.
 */
template<typename T>
class Vector3D {

 public:

  /**
   * Set the regularisation-epsilon value for **all** Vector3D objects of this
   * type.
   * @param new_eps the new regularisation-epsilon.
   */
  static void set_eps(T new_eps) {

    _eps = new_eps;
    _eps_squared = new_eps * new_eps;

  }

  /**
   * Retrieve the regularisation-epsilon.
   * @return the regularisation-epsilon.
   */
  static T eps() {

    return _eps;

  }

  /**
   * Retrieve the regularisation-epsilon squared.
   * @return the regularisation-epsilon squared.
   */
  static T eps_squared() {

    return _eps_squared;

  }

  /**
   * Create a three dimensional zero-vector object.
   */
  Vector3D() : _x{0.0, 0.0, 0.0} {}

  /**
   * Create a three dimensional vector object with the given x, y & z
   * components along with a regularisation-epsilon value.
   * @param x the vector x component.
   * @param y the vector y component.
   * @param z the vector z component.
   */
  Vector3D(T x, T y, T z) : _x{x, y, z} {}

  /**
   * Retrieve the vector's x-component.
   * @return the vector's x-component.
   */
  [[nodiscard]] inline T x() const { return _x[0]; }

  /**
   * Retrieve the vector's y-component.
   * @return the vector's y-component.
   */
  [[nodiscard]] inline T y() const { return _x[1]; }

  /**
   * Retrieve the vector's z-component.
   * @return the vector's z-component.
   */
  [[nodiscard]] inline T z() const { return _x[2]; }

  /**
   * Retrieve the vector's ith component.
   * @param idx the index of the component.
   * @return the vector component.
   */
  [[nodiscard]] T operator()(size_t idx) const { return _x[idx]; }

 private:

  std::array<T, 3> _x;

  [[maybe_unused]] static T _eps;
  [[maybe_unused]] static T _eps_squared;

};

// Initialize static eps & eps_squared values to defaults for double precision
// arithmetic.
template<typename T>
T Vector3D<T>::_eps = 1E-7;

template<typename T>
T Vector3D<T>::_eps_squared = 1E-14;

/**
 * Redirection operator to display the vector.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param out the output stream.
 * @param v the vector to display.
 * @return the output stream with a representation of the input vector.
 */
template<typename T>
std::ostream &operator<<(std::ostream &out, const Vector3D<T> v) {

  out << "<" << v.x() << ", " << v.y() << ", " << v.z() << ">";
  return out;

}

/**
 * Vector addition operator.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param u the vector on the left hand side of the sum.
 * @param v the vector on the right hand side of the sum.
 * @return the sum of the two input vectors.
 */
template<typename T>
Vector3D<T> operator+(const Vector3D<T> &u, const Vector3D<T> &v) {

  return {u.x() + v.x(), u.y() + v.y(), u.z() + v.z()};

}

/**
 * Vector subtraction operator.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param u the vector on the left hand side of the operator.
 * @param v the vector on the right hand side of the operator.
 * @return the difference of two input vectors.
 */
template<typename T>
Vector3D<T> operator-(const Vector3D<T> &u, const Vector3D<T> &v) {

  return {u.x() - v.x(), u.y() - v.y(), u.z() - v.z()};

}

/**
 * Vector-scalar product operator.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param v the vector on the left hand side of the product.
 * @param lambda the scalar on the right hand side of the product.
 * @return the vector-scalar product.
 */
template<typename T>
Vector3D<T> operator*(const Vector3D<T> &v, T lambda) {

  return {lambda * v.x(), lambda * v.y(), lambda * v.z()};

}

/**
 * Scalar-vector product operator.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param lambda the scalar on the left hand side of the product.
 * @param v the vector on the right hand side of the product.
 * @return the scalar-vector product.
 */
template<typename T>
Vector3D<T> operator*(T lambda, const Vector3D<T> &v) {

  return {lambda * v.x(), lambda * v.y(), lambda * v.z()};

}

/**
 * Vector-scalar division.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param v the vector on the left hand side of the division.
 * @param lambda the scalar on the right hand side of the division.
 * @return the vector-scalar division.
 */
template<typename T>
Vector3D<T> operator/(const Vector3D<T> &v, T lambda) {

  return {v.x() / lambda, v.y() / lambda, v.z() / lambda};

}

/**
 * Vector dot product.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param u the vector on the left hand side of the dot product.
 * @param v the scalar on the right hand side of the dot product.
 * @return the vector dot product.
 */
template<typename T>
T dot(const Vector3D<T> &u, const Vector3D<T> &v) {

  return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();

}

/**
 * Vector cross product.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param u the vector on the left hand side of the cross product.
 * @param v the scalar on the right hand side of the cross product.
 * @return the vector cross product.
 */template<typename T>
Vector3D<T> cross(const Vector3D<T> &u, const Vector3D<T> &v) {

  return {u.y() * v.z() - u.z() * v.y(),
          -u.x() * v.z() + u.z() * v.x(),
          u.x() * v.y() - u.y() * v.x()};

}

/**
 * The norm of a vector
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param v the vector for which we seek the norm.
 * @return the norm of the input vector.
 */
template<typename T>
T norm(const Vector3D<T> &v) {

  return sqrt(dot(v, v) + Vector3D<T>::eps_squared());

}

/**
 * The norm squared of a vector.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param v the vector for which we seek the norm-squared.
 * @return the norm-squared of the input vector.
 */
template<typename T>
T norm_squared(const Vector3D<T> &v) {

  return dot(v, v);

}

/**
 * Produce a normalised version of the input vector.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param v the vector that we seek to normalise.
 * @return the normalised input vector.
 */
template<typename T>
Vector3D<T> normalised(const Vector3D<T> &v) {

  T l = norm(v);
  return v / l;

}

//---------------------------------------------------------------------------//
// 4 dimensional vector                                                      //
//---------------------------------------------------------------------------//

/**
 * An implementation of a three dimensional cartesian vector.
 * @tparam T the underlying data type for the calculation - usually ‘double’ or
 *           ‘mpreal’.
 */
template<typename T>
class Vector4D {

 public:

  /**
   * Set the regularisation-epsilon value for **all** Vector4D objects of this
   * type.
   * @param new_eps the new regularisation-epsilon.
   */
  static void set_eps(T new_eps) {

    _eps = new_eps;
    _eps_squared = new_eps * new_eps;

  }

  /**
   * Retrieve the regularisation-epsilon.
   * @return the regularisation-epsilon.
   */
  static T eps() {

    return _eps;

  }

  /**
   * Retrieve the regularisation-epsilon squared.
   * @return the regularisation-epsilon squared.
   */
  static T eps_squared() {

    return _eps_squared;

  }

  /**
   * Create a three dimensional zero-vector object.
   */
  Vector4D() : _x{0.0, 0.0, 0.0, 0.0} {}

  /**
   * Create a three dimensional vector object with the given x, y & z
   * components along with a regularisation-epsilon value.
   * @param x the vector x component.
   * @param y the vector y component.
   * @param z the vector z component.
   * @param w the vector w component.
   */
  Vector4D(T x, T y, T z, T w) : _x{x, y, z, w} {}

  /**
   * Retrieve the vector's x-component.
   * @return the vector's x-component.
   */
  [[nodiscard]] inline T x() const { return _x[0]; }

  /**
   * Retrieve the vector's y-component.
   * @return the vector's y-component.
   */
  [[nodiscard]] inline T y() const { return _x[1]; }

  /**
   * Retrieve the vector's z-component.
   * @return the vector's z-component.
   */
  [[nodiscard]] inline T z() const { return _x[2]; }

  /**
   * Retrieve the vector's w-component.
   * @return the vector's w-component.
   */
  [[nodiscard]] inline T w() const { return _x[3]; }

 private:

  std::array<T, 4> _x;

  [[maybe_unused]] static T _eps;
  [[maybe_unused]] static T _eps_squared;

};

// Initialize static eps & eps_squared values to defaults for double precision
// arithmetic.
template<typename T>
T Vector4D<T>::_eps = 1E-7;

template<typename T>
T Vector4D<T>::_eps_squared = 1E-14;

/**
 * Redirection operator to display the vector.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param out the output stream.
 * @param v the vector to display.
 * @return the output stream with a representation of the input vector.
 */
template<typename T>
std::ostream &operator<<(std::ostream &out, const Vector4D<T> v) {

  out << "<" << v.x() << ", "
      << v.y() << ", "
      << v.z() << ", "
      << v.w() << ">";

  return out;

}

/**
 * Vector addition operator.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param u the vector on the left hand side of the sum.
 * @param v the vector on the right hand side of the sum.
 * @return the sum of the two input vectors.
 */
template<typename T>
Vector4D<T> operator+(const Vector4D<T> &u, const Vector4D<T> &v) {

  return {u.x() + v.x(), u.y() + v.y(), u.z() + v.z(), u.w() + v.w()};

}

/**
 * Vector subtraction operator.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param u the vector on the left hand side of the operator.
 * @param v the vector on the right hand side of the operator.
 * @return the difference of two input vectors.
 */
template<typename T>
Vector4D<T> operator-(const Vector4D<T> &u, const Vector4D<T> &v) {

  return {u.x() - v.x(), u.y() - v.y(), u.z() - v.z(), u.w() - v.w()};

}

/**
 * Vector-scalar product operator.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param v the vector on the left hand side of the product.
 * @param lambda the scalar on the right hand side of the product.
 * @return the vector-scalar product.
 */
template<typename T>
Vector4D<T> operator*(const Vector4D<T> &v, T lambda) {

  return {v.x() * lambda, v.y() * lambda, v.z() * lambda, v.w() * lambda};

}

/**
 * Scalar-vector product operator.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param lambda the scalar on the left hand side of the product.
 * @param v the vector on the right hand side of the product.
 * @return the scalar-vector product.
 */
template<typename T>
Vector4D<T> operator*(T lambda, const Vector4D<T> &v) {

  return {lambda * v.x(), lambda * v.y(), lambda * v.z(), lambda * v.w()};

}

/**
 * Vector-scalar division.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param v the vector on the left hand side of the division.
 * @param lambda the scalar on the right hand side of the division.
 * @return the vector-scalar division.
 */
template<typename T>
Vector4D<T> operator/(const Vector4D<T> &v, T lambda) {

  return {v.x() / lambda, v.y() / lambda, v.z() / lambda, v.w() / lambda};

}

/**
 * Vector dot product.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param u the vector on the left hand side of the dot product.
 * @param v the scalar on the right hand side of the dot product.
 * @return the vector dot product.
 */
template<typename T>
T dot(const Vector4D<T> &u, const Vector4D<T> &v) {

  return u.x() * v.x() + u.y() * v.y() + u.z() * v.z() + u.w() * v.w();

}

/**
 * The norm of a vector
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param v the vector for which we seek the norm.
 * @return the norm of the input vector.
 */
template<typename T>
T norm(const Vector4D<T> &v) {

  return sqrt(dot(v, v) + Vector4D<T>::eps_squared());

}

/**
 * The norm squared of a vector.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param v the vector for which we seek the norm-squared.
 * @return the norm-squared of the input vector.
 */
template<typename T>
T norm_squared(const Vector4D<T> &v) {

  return dot(v, v);

}

/**
 * Produce a normalised version of the input vector.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param v the vector that we seek to normalise.
 * @return the normalised input vector.
 */
template<typename T>
Vector4D<T> normalised(const Vector4D<T> &v) {

  T l = norm(v);
  return v / l;

}

//---------------------------------------------------------------------------//
// 3x3 Matrix                                                                //
//---------------------------------------------------------------------------//

/**
 * An implementation of a three by three square matrix.
 */
template<typename T>
class Matrix3x3 {

 public:

  /**
   * Create a 3x3 zero-matrix.
   */
  Matrix3x3() :
      _m{{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}} {}

  /**
   * Create a 3x3 matrix from an input.
   */
  Matrix3x3(std::initializer_list<std::initializer_list<T>> m) {

    // assert(m.size() == 3 && "Number of rows must be 3.");

    auto outerIterator = m.begin();
    auto innerIterator = outerIterator->begin();

    assert(outerIterator->size() == 3 && "Number of columns must be 3.");
    _m[0][0] = *innerIterator++;
    _m[0][1] = *innerIterator++;
    _m[0][2] = *innerIterator++;

    outerIterator++;
    innerIterator = outerIterator->begin();

    assert(outerIterator->size() == 3 && "Number of columns must be 3.");
    _m[1][0] = *innerIterator++;
    _m[1][1] = *innerIterator++;
    _m[1][2] = *innerIterator++;

    outerIterator++;
    innerIterator = outerIterator->begin();

    assert(outerIterator->size() == 3 && "Number of columns must be 3.");
    _m[2][0] = *innerIterator++;
    _m[2][1] = *innerIterator++;
    _m[2][2] = *innerIterator++;

  }

  Matrix3x3 &operator=(const Matrix3x3 &m) {

    if (this == &m) return *this;
    _m[0][0] = m._m[0][0];
    _m[0][1] = m._m[0][1];
    _m[0][2] = m._m[0][2];

    _m[1][0] = m._m[1][0];
    _m[1][1] = m._m[1][1];
    _m[1][2] = m._m[1][2];

    _m[2][0] = m._m[2][0];
    _m[2][1] = m._m[2][1];
    _m[2][2] = m._m[2][2];

    return *this;

  }

  [[nodiscard]]
  T operator()(size_t i, size_t j) const { return _m[i][j]; }

 private:

  T _m[3][3];

};

/**
 * Redirection operator to display the matrix.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param out the output stream.
 * @param m the matrix to display.
 * @return the output stream with a representation of the input matrix.
 */
template<typename T>
std::ostream &operator<<(std::ostream &out, const Matrix3x3<T> m) {

  out << m(0, 0) << " " << m(0, 1) << " " << m(0, 2) << std::endl;
  out << m(1, 0) << " " << m(1, 1) << " " << m(1, 2) << std::endl;
  out << m(2, 0) << " " << m(2, 1) << " " << m(2, 2) << std::endl;

  return out;

}

/**
 * Matrix addition operator.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param u the matrix on the left hand side of the sum.
 * @param v the matrix on the right hand side of the sum.
 * @return the sum of the two input matrices.
 */
template<typename T>
Matrix3x3<T> operator+(const Matrix3x3<T> &u, const Matrix3x3<T> &v) {

  return {{u(0, 0) + v(0, 0), u(0, 1) + v(0, 1), u(0, 2) + v(0, 2)},
          {u(1, 0) + v(1, 0), u(1, 1) + v(1, 1), u(1, 2) + v(1, 2)},
          {u(2, 0) + v(2, 0), u(2, 1) + v(2, 1), u(2, 2) + v(2, 2)}};

}

/**
 * Matrix subtraction operator.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param u the matrix on the left hand side of the operator.
 * @param v the matrix on the right hand side of the operator.
 * @return the difference of two input matrices.
 */
template<typename T>
Matrix3x3<T> operator-(const Matrix3x3<T> &u, const Matrix3x3<T> &v) {

  return {{u(0, 0) - v(0, 0), u(0, 1) - v(0, 1), u(0, 2) - v(0, 2)},
          {u(1, 0) - v(1, 0), u(1, 1) - v(1, 1), u(1, 2) - v(1, 2)},
          {u(2, 0) - v(2, 0), u(2, 1) - v(2, 1), u(2, 2) - v(2, 2)}};

}

/**
 * Matrix-scalar product operator.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param v the matrix on the left hand side of the product.
 * @param lambda the scalar on the right hand side of the product.
 * @return the matrix-scalar product.
 */
template<typename T>
Matrix3x3<T> operator*(const Matrix3x3<T> &v, T lambda) {

  return {{v(0, 0) * lambda, v(0, 1) * lambda, v(0, 2) * lambda},
          {v(1, 0) * lambda, v(1, 1) * lambda, v(1, 2) * lambda},
          {v(2, 0) * lambda, v(2, 1) * lambda, v(2, 2) * lambda}};

}

/**
 * Scalar-matrix product operator.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param lambda the scalar on the left hand side of the product.
 * @param v the matrix on the right hand side of the product.
 * @return the scalar-matrix product.
 */
template<typename T>
Matrix3x3<T> operator*(T lambda, const Matrix3x3<T> &v) {

  return {{lambda * v(0, 0), lambda * v(0, 1), lambda * v(0, 2)},
          {lambda * v(1, 0), lambda * v(1, 1), lambda * v(1, 2)},
          {lambda * v(2, 0), lambda * v(2, 1), lambda * v(2, 2)}};

}

/**
 * Matrix-scalar division.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param v the matrix on the left hand side of the division.
 * @param lambda the scalar on the right hand side of the division.
 * @return the matrix-scalar division.
 */
template<typename T>
Matrix3x3<T> operator/(const Matrix3x3<T> &v, T lambda) {

  return {{v(0, 0) / lambda, v(0, 1) / lambda, v(0, 2) / lambda},
          {v(1, 0) / lambda, v(1, 1) / lambda, v(1, 2) / lambda},
          {v(2, 0) / lambda, v(2, 1) / lambda, v(2, 2) / lambda}};

}

/**
 * Matrix-vector multiplication.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param m the matrix on the left hand side of the multiplication.
 * @param v the vector on the right hand side of the multiplication.
 * @return the matrix-vector multiplication.
 */
template<typename T>
Vector3D<T> operator*(const Matrix3x3<T> &m, const Vector3D<T> &v) {

  return {m(0, 0) * v.x() + m(0, 1) * v.y() + m(0, 2) * v.z(),
          m(1, 0) * v.x() + m(1, 1) * v.y() + m(1, 2) * v.z(),
          m(2, 0) * v.x() + m(2, 1) * v.y() + m(2, 2) * v.z()};

}

/**
 * Vector-matrix multiplication.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param v the vector on the left hand side of the multiplication.
 * @param m the matrix on the right hand side of the multiplication.
 * @return the matrix-vector multiplication.
 */
template<typename T>
Vector3D<T> operator*(const Vector3D<T> &v, const Matrix3x3<T> &m) {

  return {m(0, 0) * v.x() + m(1, 0) * v.y() + m(2, 0) * v.z(),
          m(0, 1) * v.x() + m(1, 1) * v.y() + m(2, 1) * v.z(),
          m(0, 2) * v.x() + m(1, 2) * v.y() + m(2, 2) * v.z()};

}

/**
 * Matrix-matrix multiplication.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param m0 the matrix on the left hand side of the multiplication.
 * @param m1 the matrix on the right hand side of the multiplication.
 * @return the matrix-matrix multiplication.
 */
template<typename T>
Matrix3x3<T> operator*(const Matrix3x3<T> &m0, const Matrix3x3<T> &m1) {

  return {{m0(0, 0) * m1(0, 0) + m0(0, 1) * m1(1, 0) + m0(0, 2) * m1(2, 0),
           m0(0, 0) * m1(0, 1) + m0(0, 1) * m1(1, 1) + m0(0, 2) * m1(2, 1),
           m0(0, 0) * m1(0, 2) + m0(0, 1) * m1(1, 2) + m0(0, 2) * m1(2, 2)},

          {m0(1, 0) * m1(0, 0) + m0(1, 1) * m1(1, 0) + m0(1, 2) * m1(2, 0),
           m0(1, 0) * m1(0, 1) + m0(1, 1) * m1(1, 1) + m0(1, 2) * m1(2, 1),
           m0(1, 0) * m1(0, 2) + m0(1, 1) * m1(1, 2) + m0(1, 2) * m1(2, 2)},

          {m0(2, 0) * m1(0, 0) + m0(2, 1) * m1(1, 0) + m0(2, 2) * m1(2, 0),
           m0(2, 0) * m1(0, 1) + m0(2, 1) * m1(1, 1) + m0(2, 2) * m1(2, 1),
           m0(2, 0) * m1(0, 2) + m0(2, 1) * m1(1, 2) + m0(2, 2) * m1(2, 2)}};

}

/**
 * Matrix adjugate.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param m the matrix.
 * @return the matrix adjugate.
 */
template<typename T>
Matrix3x3<T> adj(const Matrix3x3<T> &m) {

  return {{-(m(1, 2) * m(2, 1)) + m(1, 1) * m(2, 2),
           m(0, 2) * m(2, 1) - m(0, 1) * m(2, 2),
           -(m(0, 2) * m(1, 1)) + m(0, 1) * m(1, 2)},

          {m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2),
           -(m(0, 2) * m(2, 0)) + m(0, 0) * m(2, 2),
           m(0, 2) * m(1, 0) - m(0, 0) * m(1, 2)},

          {-(m(1, 1) * m(2, 0)) + m(1, 0) * m(2, 1),
           m(0, 1) * m(2, 0) - m(0, 0) * m(2, 1),
           -(m(0, 1) * m(1, 0)) + m(0, 0) * m(1, 1)}};

}

/**
 * Matrix determinant.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param m the matrix.
 * @return the matrix determinant.
 */
template<typename T>
T det(const Matrix3x3<T> &m) {

  return -(m(0, 2) * m(1, 1) * m(2, 0)) + m(0, 1) * m(1, 2) * m(2, 0) +
      m(0, 2) * m(1, 0) * m(2, 1) - m(0, 0) * m(1, 2) * m(2, 1) -
      m(0, 1) * m(1, 0) * m(2, 2) + m(0, 0) * m(1, 1) * m(2, 2);

}

/**
 * Matrix transpose.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param m the matrix.
 * @return the matrix transpose.
 */
template<typename T>
Matrix3x3<T> tr(const Matrix3x3<T> &m) {

  return {{m(0, 0), m(1, 0), m(2, 0)},
          {m(0, 1), m(1, 1), m(2, 1)},
          {m(0, 2), m(1, 2), m(2, 2)}};

}

/**
 * Matrix-matrix Frobenius (dot) product.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param u the matrix on the left hand side of the dot product.
 * @param v the matrix on the right hand side of the dot product.
 * @return the matrix-vector multiplication.
 */
template<typename T>
T dot(const Matrix3x3<T> &u, const Matrix3x3<T> &v) {

  return u(0, 0) * v(0, 0) + u(0, 1) * v(0, 1) + u(0, 2) * v(0, 2) +
      u(1, 0) * v(1, 0) + u(1, 1) * v(1, 1) + u(1, 2) * v(1, 2) +
      u(2, 0) * v(2, 0) + u(2, 1) * v(2, 1) + u(2, 2) * v(2, 2);

}

/**
 * Matrix Frobenius norm.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param u the matrix to take the norm of.
 * @return the matrix Frobenius norm.
 */
template<typename T>
T norm(const Matrix3x3<T> &u) {

  return sqrt(dot(u, u));

}

/**
 * Matrix diagonal.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param m the matrix from which to extract the diagonal.
 * @return the matrix diagonal.
 */
template<typename T>
Vector3D<T> diag(const Matrix3x3<T> &m) {

  return {m(0, 0), m(1, 1), m(2, 2)};

}

/**
 * Matrix trace.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param m the matrix for which to compute the trace.
 * @return the matrix trace.
 */
template<typename T>
T trace(const Matrix3x3<T> &m) {

  return m(0, 0) + m(1, 1) + m(2, 2);

}

template<typename T>
Matrix3x3<T> rotation(T gamma, const Vector3D<T> &axis) {

  auto naxis = normalised(axis);

  return {{pow(naxis.x(), 2) * (1 - cos(gamma)) + cos(gamma),
           naxis.x() * naxis.y() * (1 - cos(gamma)) - naxis.z() * sin(gamma),
           naxis.x() * naxis.z() * (1 - cos(gamma)) + naxis.y() * sin(gamma)},

          {naxis.x() * naxis.y() * (1 - cos(gamma)) + naxis.z() * sin(gamma),
           pow(naxis.y(), 2) * (1 - cos(gamma)) + cos(gamma),
           naxis.y() * naxis.z() * (1 - cos(gamma)) - naxis.x() * sin(gamma)},

          {naxis.x() * naxis.z() * (1 - cos(gamma)) - naxis.y() * sin(gamma),
           naxis.y() * naxis.z() * (1 - cos(gamma)) + naxis.x() * sin(gamma),
           pow(naxis.z(), 2) * (1 - cos(gamma)) + cos(gamma)}
  };

}

//---------------------------------------------------------------------------//
// 4x4 Matrix                                                                //
//---------------------------------------------------------------------------//

template<typename T>
class Matrix4x4 {

 public:

  /**
   * Create a 4x4 zero-matrix.
   */
  Matrix4x4() :
      _m{{0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}} {}

  /**
   * Create a 4x4 matrix from an input.
   */
  Matrix4x4(std::initializer_list<std::initializer_list<T>> m) {

    assert(m.size() == 4 && "Number of rows must be 4.");

    auto outerIterator = m.begin();
    auto innerIterator = outerIterator->begin();

    assert(outerIterator->size() == 4 && "Number of columns must be 4.");
    _m[0][0] = *innerIterator++;
    _m[0][1] = *innerIterator++;
    _m[0][2] = *innerIterator++;
    _m[0][3] = *innerIterator++;

    outerIterator++;
    innerIterator = outerIterator->begin();

    assert(outerIterator->size() == 4 && "Number of columns must be 4.");
    _m[1][0] = *innerIterator++;
    _m[1][1] = *innerIterator++;
    _m[1][2] = *innerIterator++;
    _m[1][3] = *innerIterator++;

    outerIterator++;
    innerIterator = outerIterator->begin();

    assert(outerIterator->size() == 4 && "Number of columns must be 4.");
    _m[2][0] = *innerIterator++;
    _m[2][1] = *innerIterator++;
    _m[2][2] = *innerIterator++;
    _m[2][3] = *innerIterator++;

    outerIterator++;
    innerIterator = outerIterator->begin();

    assert(outerIterator->size() == 4 && "Number of columns must be 4.");
    _m[3][0] = *innerIterator++;
    _m[3][1] = *innerIterator++;
    _m[3][2] = *innerIterator++;
    _m[3][3] = *innerIterator++;

  }

  Matrix4x4 &operator=(const Matrix4x4 &m) {

    if (this == &m) return *this;
    _m[0][0] = m._m[0][0];
    _m[0][1] = m._m[0][1];
    _m[0][2] = m._m[0][2];
    _m[0][3] = m._m[0][3];

    _m[1][0] = m._m[1][0];
    _m[1][1] = m._m[1][1];
    _m[1][2] = m._m[1][2];
    _m[1][3] = m._m[1][3];

    _m[2][0] = m._m[2][0];
    _m[2][1] = m._m[2][1];
    _m[2][2] = m._m[2][2];
    _m[2][3] = m._m[2][3];

    _m[3][0] = m._m[3][0];
    _m[3][1] = m._m[3][1];
    _m[3][2] = m._m[3][2];
    _m[3][3] = m._m[3][3];

    return *this;

  }

  [[nodiscard]]
  T operator()(size_t i, size_t j) const { return _m[i][j]; }

 private:

  T _m[4][4];

};

/**
 * Redirection operator to display the matrix.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param out the output stream.
 * @param m the matrix to display.
 * @return the output stream with a representation of the input matrix.
 */
template<typename T>
std::ostream &operator<<(std::ostream &out, const Matrix4x4<T> m) {

  out << m(0, 0) << " " << m(0, 1) << " " << m(0, 2) << " " << m(0, 3)
      << std::endl;
  out << m(1, 0) << " " << m(1, 1) << " " << m(1, 2) << " " << m(1, 3)
      << std::endl;
  out << m(2, 0) << " " << m(2, 1) << " " << m(2, 2) << " " << m(2, 3)
      << std::endl;
  out << m(3, 0) << " " << m(3, 1) << " " << m(3, 2) << " " << m(3, 3)
      << std::endl;

  return out;

}

/**
 * Matrix addition operator.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param u the matrix on the left hand side of the sum.
 * @param v the matrix on the right hand side of the sum.
 * @return the sum of the two input matrices.
 */
template<typename T>
Matrix4x4<T> operator+(const Matrix4x4<T> &u, const Matrix4x4<T> &v) {

  return {{u(0, 0) + v(0, 0), u(0, 1) + v(0, 1),
           u(0, 2) + v(0, 2), u(0, 3) + v(0, 3)},
          {u(1, 0) + v(1, 0), u(1, 1) + v(1, 1),
           u(1, 2) + v(1, 2), u(1, 3) + v(1, 3)},
          {u(2, 0) + v(2, 0), u(2, 1) + v(2, 1),
           u(2, 2) + v(2, 2), u(2, 3) + v(2, 3)},
          {u(3, 0) + v(3, 0), u(3, 1) + v(3, 1),
           u(3, 2) + v(3, 2), u(3, 3) + v(3, 3)}
  };

}

/**
 * Matrix subtraction operator.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param u the matrix on the left hand side of the sum.
 * @param v the matrix on the right hand side of the sum.
 * @return the difference of the two input matrices.
 */
template<typename T>
Matrix4x4<T> operator-(const Matrix4x4<T> &u, const Matrix4x4<T> &v) {

  return {{u(0, 0) - v(0, 0), u(0, 1) - v(0, 1),
           u(0, 2) - v(0, 2), u(0, 3) - v(0, 3)},
          {u(1, 0) - v(1, 0), u(1, 1) - v(1, 1),
           u(1, 2) - v(1, 2), u(1, 3) - v(1, 3)},
          {u(2, 0) - v(2, 0), u(2, 1) - v(2, 1),
           u(2, 2) - v(2, 2), u(2, 3) - v(2, 3)},
          {u(3, 0) - v(3, 0), u(3, 1) - v(3, 1),
           u(3, 2) - v(3, 2), u(3, 3) - v(3, 3)}
  };

}

/**
 * Scalar-matrix product operator.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param lambda the scalar on the right hand side of the product.
 * @param v the matrix on the left hand side of the product.
 * @return the matrix-scalar product.
 */
template<typename T>
Matrix4x4<T> operator*(T lambda, const Matrix4x4<T> &v) {

  return {
      {lambda * v(0, 0), lambda * v(0, 1), lambda * v(0, 2), lambda * v(0, 3)},
      {lambda * v(1, 0), lambda * v(1, 1), lambda * v(1, 2), lambda * v(1, 3)},
      {lambda * v(2, 0), lambda * v(2, 1), lambda * v(2, 2), lambda * v(2, 3)},
      {lambda * v(3, 0), lambda * v(3, 1), lambda * v(3, 2), lambda * v(3, 3)}};

}

/**
 * Matrix-scalar product operator.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param v the matrix on the left hand side of the product.
 * @param lambda the scalar on the right hand side of the product.
 * @return the matrix-scalar product.
 */
template<typename T>
Matrix4x4<T> operator*(const Matrix4x4<T> &v, T lambda) {

  return {
      {v(0, 0) * lambda, v(0, 1) * lambda, v(0, 2) * lambda, v(0, 3) * lambda},
      {v(1, 0) * lambda, v(1, 1) * lambda, v(1, 2) * lambda, v(1, 3) * lambda},
      {v(2, 0) * lambda, v(2, 1) * lambda, v(2, 2) * lambda, v(2, 3) * lambda},
      {v(3, 0) * lambda, v(3, 1) * lambda, v(3, 2) * lambda, v(3, 3) * lambda}};

}

/**
 * Matrix-scalar division operator.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param v the matrix on the left hand side of the product.
 * @param lambda the scalar on the right hand side of the product.
 * @return the matrix-scalar division.
 */
template<typename T>
Matrix4x4<T> operator/(const Matrix4x4<T> &v, T lambda) {

  return {
      {v(0, 0) / lambda, v(0, 1) / lambda, v(0, 2) / lambda, v(0, 3) / lambda},
      {v(1, 0) / lambda, v(1, 1) / lambda, v(1, 2) / lambda, v(1, 3) / lambda},
      {v(2, 0) / lambda, v(2, 1) / lambda, v(2, 2) / lambda, v(2, 3) / lambda},
      {v(3, 0) / lambda, v(3, 1) / lambda, v(3, 2) / lambda, v(3, 3) / lambda}};

}

/**
 * Matrix-vector multiplication.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param m the matrix on the left hand side of the multiplication.
 * @param v the vector on the right hand side of the multiplication.
 * @return the matrix-vector multiplication.
 */
template<typename T>
Vector4D<T> operator*(const Matrix4x4<T> &m, const Vector4D<T> &v) {

  return
      {m(0, 3) * v.w() + m(0, 0) * v.x() + m(0, 1) * v.y() + m(0, 2) * v.z(),
       m(1, 3) * v.w() + m(1, 0) * v.x() + m(1, 1) * v.y() + m(1, 2) * v.z(),
       m(2, 3) * v.w() + m(2, 0) * v.x() + m(2, 1) * v.y() + m(2, 2) * v.z(),
       m(3, 3) * v.w() + m(3, 0) * v.x() + m(3, 1) * v.y() + m(3, 2) * v.z()};

}

/**
 * Vector-matrix multiplication.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param v the vector on the left hand side of the multiplication.
 * @param m the matrix on the right hand side of the multiplication.
 * @return the matrix-vector multiplication.
 */
template<typename T>
Vector4D<T> operator*(const Vector4D<T> &v, const Matrix4x4<T> &m) {

  return
      {m(3, 0) * v.w() + m(0, 0) * v.x() + m(1, 0) * v.y() + m(2, 0) * v.z(),
       m(3, 1) * v.w() + m(0, 1) * v.x() + m(1, 1) * v.y() + m(2, 1) * v.z(),
       m(3, 2) * v.w() + m(0, 2) * v.x() + m(1, 2) * v.y() + m(2, 2) * v.z(),
       m(3, 3) * v.w() + m(0, 3) * v.x() + m(1, 3) * v.y() + m(2, 3) * v.z()};

}

/**
 * Matrix-matrix multiplication.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param m0 the matrix on the left hand side of the multiplication.
 * @param m1 the matrix on the right hand side of the multiplication.
 * @return the matrix-matrix multiplication.
 */
template<typename T>
Matrix4x4<T> operator*(const Matrix4x4<T> &m0, const Matrix4x4<T> &m1) {

  return
      {{m0(0, 0) * m1(0, 0) + m0(0, 1) * m1(1, 0)
            + m0(0, 2) * m1(2, 0) + m0(0, 3) * m1(3, 0),
        m0(0, 0) * m1(0, 1) + m0(0, 1) * m1(1, 1)
            + m0(0, 2) * m1(2, 1) + m0(0, 3) * m1(3, 1),
        m0(0, 0) * m1(0, 2) + m0(0, 1) * m1(1, 2)
            + m0(0, 2) * m1(2, 2) + m0(0, 3) * m1(3, 2),
        m0(0, 0) * m1(0, 3) + m0(0, 1) * m1(1, 3)
            + m0(0, 2) * m1(2, 3) + m0(0, 3) * m1(3, 3)},

       {m0(1, 0) * m1(0, 0) + m0(1, 1) * m1(1, 0)
            + m0(1, 2) * m1(2, 0) + m0(1, 3) * m1(3, 0),
        m0(1, 0) * m1(0, 1) + m0(1, 1) * m1(1, 1)
            + m0(1, 2) * m1(2, 1) + m0(1, 3) * m1(3, 1),
        m0(1, 0) * m1(0, 2) + m0(1, 1) * m1(1, 2)
            + m0(1, 2) * m1(2, 2) + m0(1, 3) * m1(3, 2),
        m0(1, 0) * m1(0, 3) + m0(1, 1) * m1(1, 3)
            + m0(1, 2) * m1(2, 3) + m0(1, 3) * m1(3, 3)},

       {m0(2, 0) * m1(0, 0) + m0(2, 1) * m1(1, 0)
            + m0(2, 2) * m1(2, 0) + m0(2, 3) * m1(3, 0),
        m0(2, 0) * m1(0, 1) + m0(2, 1) * m1(1, 1)
            + m0(2, 2) * m1(2, 1) + m0(2, 3) * m1(3, 1),
        m0(2, 0) * m1(0, 2) + m0(2, 1) * m1(1, 2)
            + m0(2, 2) * m1(2, 2) + m0(2, 3) * m1(3, 2),
        m0(2, 0) * m1(0, 3) + m0(2, 1) * m1(1, 3)
            + m0(2, 2) * m1(2, 3) + m0(2, 3) * m1(3, 3)},

       {m0(3, 0) * m1(0, 0) + m0(3, 1) * m1(1, 0)
            + m0(3, 2) * m1(2, 0) + m0(3, 3) * m1(3, 0),
        m0(3, 0) * m1(0, 1) + m0(3, 1) * m1(1, 1)
            + m0(3, 2) * m1(2, 1) + m0(3, 3) * m1(3, 1),
        m0(3, 0) * m1(0, 2) + m0(3, 1) * m1(1, 2)
            + m0(3, 2) * m1(2, 2) + m0(3, 3) * m1(3, 2),
        m0(3, 0) * m1(0, 3) + m0(3, 1) * m1(1, 3)
            + m0(3, 2) * m1(2, 3) + m0(3, 3) * m1(3, 3)}};

}

/**
 * Matrix adjugate.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param m the matrix.
 * @return the matrix adjugate.
 */
template<typename T>
Matrix4x4<T> adj(const Matrix4x4<T> &m) {

  return {
      {-(m(1, 3) * m(2, 2) * m(3, 1)) + m(1, 2) * m(2, 3) * m(3, 1)
           + m(1, 3) * m(2, 1) * m(3, 2) - m(1, 1) * m(2, 3) * m(3, 2)
           - m(1, 2) * m(2, 1) * m(3, 3) + m(1, 1) * m(2, 2) * m(3, 3),
       m(0, 3) * m(2, 2) * m(3, 1) - m(0, 2) * m(2, 3) * m(3, 1)
           - m(0, 3) * m(2, 1) * m(3, 2) + m(0, 1) * m(2, 3) * m(3, 2)
           + m(0, 2) * m(2, 1) * m(3, 3) - m(0, 1) * m(2, 2) * m(3, 3),
       -(m(0, 3) * m(1, 2) * m(3, 1)) + m(0, 2) * m(1, 3) * m(3, 1)
           + m(0, 3) * m(1, 1) * m(3, 2) - m(0, 1) * m(1, 3) * m(3, 2)
           - m(0, 2) * m(1, 1) * m(3, 3) + m(0, 1) * m(1, 2) * m(3, 3),
       m(0, 3) * m(1, 2) * m(2, 1) - m(0, 2) * m(1, 3) * m(2, 1)
           - m(0, 3) * m(1, 1) * m(2, 2) + m(0, 1) * m(1, 3) * m(2, 2)
           + m(0, 2) * m(1, 1) * m(2, 3) - m(0, 1) * m(1, 2) * m(2, 3)},

      {m(1, 3) * m(2, 2) * m(3, 0) - m(1, 2) * m(2, 3) * m(3, 0)
           - m(1, 3) * m(2, 0) * m(3, 2) + m(1, 0) * m(2, 3) * m(3, 2)
           + m(1, 2) * m(2, 0) * m(3, 3) - m(1, 0) * m(2, 2) * m(3, 3),
       -(m(0, 3) * m(2, 2) * m(3, 0)) + m(0, 2) * m(2, 3) * m(3, 0)
           + m(0, 3) * m(2, 0) * m(3, 2) - m(0, 0) * m(2, 3) * m(3, 2)
           - m(0, 2) * m(2, 0) * m(3, 3) + m(0, 0) * m(2, 2) * m(3, 3),
       m(0, 3) * m(1, 2) * m(3, 0) - m(0, 2) * m(1, 3) * m(3, 0)
           - m(0, 3) * m(1, 0) * m(3, 2) + m(0, 0) * m(1, 3) * m(3, 2)
           + m(0, 2) * m(1, 0) * m(3, 3) - m(0, 0) * m(1, 2) * m(3, 3),
       -(m(0, 3) * m(1, 2) * m(2, 0)) + m(0, 2) * m(1, 3) * m(2, 0)
           + m(0, 3) * m(1, 0) * m(2, 2) - m(0, 0) * m(1, 3) * m(2, 2)
           - m(0, 2) * m(1, 0) * m(2, 3) + m(0, 0) * m(1, 2) * m(2, 3)},

      {-(m(1, 3) * m(2, 1) * m(3, 0)) + m(1, 1) * m(2, 3) * m(3, 0)
           + m(1, 3) * m(2, 0) * m(3, 1) - m(1, 0) * m(2, 3) * m(3, 1)
           - m(1, 1) * m(2, 0) * m(3, 3) + m(1, 0) * m(2, 1) * m(3, 3),
       m(0, 3) * m(2, 1) * m(3, 0) - m(0, 1) * m(2, 3) * m(3, 0)
           - m(0, 3) * m(2, 0) * m(3, 1) + m(0, 0) * m(2, 3) * m(3, 1)
           + m(0, 1) * m(2, 0) * m(3, 3) - m(0, 0) * m(2, 1) * m(3, 3),
       -(m(0, 3) * m(1, 1) * m(3, 0)) + m(0, 1) * m(1, 3) * m(3, 0)
           + m(0, 3) * m(1, 0) * m(3, 1) - m(0, 0) * m(1, 3) * m(3, 1)
           - m(0, 1) * m(1, 0) * m(3, 3) + m(0, 0) * m(1, 1) * m(3, 3),
       m(0, 3) * m(1, 1) * m(2, 0) - m(0, 1) * m(1, 3) * m(2, 0)
           - m(0, 3) * m(1, 0) * m(2, 1) + m(0, 0) * m(1, 3) * m(2, 1)
           + m(0, 1) * m(1, 0) * m(2, 3) - m(0, 0) * m(1, 1) * m(2, 3)},

      {m(1, 2) * m(2, 1) * m(3, 0) - m(1, 1) * m(2, 2) * m(3, 0)
           - m(1, 2) * m(2, 0) * m(3, 1) + m(1, 0) * m(2, 2) * m(3, 1)
           + m(1, 1) * m(2, 0) * m(3, 2) - m(1, 0) * m(2, 1) * m(3, 2),
       -(m(0, 2) * m(2, 1) * m(3, 0)) + m(0, 1) * m(2, 2) * m(3, 0)
           + m(0, 2) * m(2, 0) * m(3, 1) - m(0, 0) * m(2, 2) * m(3, 1)
           - m(0, 1) * m(2, 0) * m(3, 2) + m(0, 0) * m(2, 1) * m(3, 2),
       m(0, 2) * m(1, 1) * m(3, 0) - m(0, 1) * m(1, 2) * m(3, 0)
           - m(0, 2) * m(1, 0) * m(3, 1) + m(0, 0) * m(1, 2) * m(3, 1)
           + m(0, 1) * m(1, 0) * m(3, 2) - m(0, 0) * m(1, 1) * m(3, 2),
       -(m(0, 2) * m(1, 1) * m(2, 0)) + m(0, 1) * m(1, 2) * m(2, 0)
           + m(0, 2) * m(1, 0) * m(2, 1) - m(0, 0) * m(1, 2) * m(2, 1)
           - m(0, 1) * m(1, 0) * m(2, 2) + m(0, 0) * m(1, 1) * m(2, 2)}
  };

}

/**
 * Matrix determinant.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param m the matrix.
 * @return the matrix determinant.
 */
template<typename T>
T det(const Matrix4x4<T> &m) {

  return m(0, 3) * m(1, 2) * m(2, 1) * m(3, 0)
      - m(0, 2) * m(1, 3) * m(2, 1) * m(3, 0) -
      m(0, 3) * m(1, 1) * m(2, 2) * m(3, 0)
      + m(0, 1) * m(1, 3) * m(2, 2) * m(3, 0) +
      m(0, 2) * m(1, 1) * m(2, 3) * m(3, 0)
      - m(0, 1) * m(1, 2) * m(2, 3) * m(3, 0) -
      m(0, 3) * m(1, 2) * m(2, 0) * m(3, 1)
      + m(0, 2) * m(1, 3) * m(2, 0) * m(3, 1) +
      m(0, 3) * m(1, 0) * m(2, 2) * m(3, 1)
      - m(0, 0) * m(1, 3) * m(2, 2) * m(3, 1) -
      m(0, 2) * m(1, 0) * m(2, 3) * m(3, 1)
      + m(0, 0) * m(1, 2) * m(2, 3) * m(3, 1) +
      m(0, 3) * m(1, 1) * m(2, 0) * m(3, 2)
      - m(0, 1) * m(1, 3) * m(2, 0) * m(3, 2) -
      m(0, 3) * m(1, 0) * m(2, 1) * m(3, 2)
      + m(0, 0) * m(1, 3) * m(2, 1) * m(3, 2) +
      m(0, 1) * m(1, 0) * m(2, 3) * m(3, 2)
      - m(0, 0) * m(1, 1) * m(2, 3) * m(3, 2) -
      m(0, 2) * m(1, 1) * m(2, 0) * m(3, 3)
      + m(0, 1) * m(1, 2) * m(2, 0) * m(3, 3) +
      m(0, 2) * m(1, 0) * m(2, 1) * m(3, 3)
      - m(0, 0) * m(1, 2) * m(2, 1) * m(3, 3) -
      m(0, 1) * m(1, 0) * m(2, 2) * m(3, 3)
      + m(0, 0) * m(1, 1) * m(2, 2) * m(3, 3);

}

/**
 * Matrix transpose.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param m the matrix.
 * @return the matrix transpose.
 */
template<typename T>
Matrix4x4<T> tr(const Matrix4x4<T> &m) {

  return {{m(0, 0), m(1, 0), m(2, 0), m(3, 0)},
          {m(0, 1), m(1, 1), m(2, 1), m(3, 1)},
          {m(0, 2), m(1, 2), m(2, 2), m(3, 2)},
          {m(0, 3), m(1, 3), m(2, 3), m(3, 3)}};

}

/**
 * Matrix-matrix Frobenius (dot) product.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param m0 the matrix on the left hand side of the dot product.
 * @param m1 the matrix on the right hand side of the dot product.
 * @return the matrix-vector multiplication.
 */
template<typename T>
T dot(const Matrix4x4<T> &m0, const Matrix4x4<T> &m1) {

  return m0(0, 0) * m1(0, 0) + m0(0, 1) * m1(0, 1)
      + m0(0, 2) * m1(0, 2) + m0(0, 3) * m1(0, 3) +
      m0(1, 0) * m1(1, 0) + m0(1, 1) * m1(1, 1)
      + m0(1, 2) * m1(1, 2) + m0(1, 3) * m1(1, 3) +
      m0(2, 0) * m1(2, 0) + m0(2, 1) * m1(2, 1)
      + m0(2, 2) * m1(2, 2) + m0(2, 3) * m1(2, 3) +
      m0(3, 0) * m1(3, 0) + m0(3, 1) * m1(3, 1)
      + m0(3, 2) * m1(3, 2) + m0(3, 3) * m1(3, 3);

}

/**
 * Matrix Frobenius product.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param m the matrix.
 * @return the matrix Frobenius norm.
 */
template<typename T>
T norm(const Matrix4x4<T> &m) {

  return sqrt(dot(m, m));

}

/**
 * Matrix diagonal.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param m the matrix from which to extract the diagonal.
 * @return the matrix diagonal.
 */
template<typename T>
Vector4D<T> diag(const Matrix4x4<T> &m) {

  return {m(0, 0), m(1, 1), m(2, 2), m(3, 3)};

}


template<typename T>
Matrix4x4<T> rotation(T gamma, const Vector4D<T> &axis) {

  auto naxis = normalised(axis);

  return {{pow(naxis.x(), 2) * (1 - cos(gamma)) + cos(gamma),
           naxis.x() * naxis.y() * (1 - cos(gamma)) - naxis.z() * sin(gamma),
           naxis.x() * naxis.z() * (1 - cos(gamma)) + naxis.y() * sin(gamma),
           0.0},

          {naxis.x() * naxis.y() * (1 - cos(gamma)) + naxis.z() * sin(gamma),
           pow(naxis.y(), 2) * (1 - cos(gamma)) + cos(gamma),
           naxis.y() * naxis.z() * (1 - cos(gamma)) - naxis.x() * sin(gamma),
           0.0},

          {naxis.x() * naxis.z() * (1 - cos(gamma)) - naxis.y() * sin(gamma),
           naxis.y() * naxis.z() * (1 - cos(gamma)) + naxis.x() * sin(gamma),
           pow(naxis.z(), 2) * (1 - cos(gamma)) + cos(gamma),
           0.0},

          {0.0, 0.0, 0.0, 1.0}
  };

}


//--------------------------------------------------------------------------//
// Plane object
//--------------------------------------------------------------------------//

/**
 * Matrix trace.
 * @tparam T the underlying data type for the calculation - usually 'double' or
 *           'mpreal'.
 * @param m the matrix for which to compute the trace.
 * @return the matrix trace.
 */
template<typename T>
T trace(const Matrix4x4<T> &m) {

  return m(0, 0) + m(1, 1) + m(2, 2) + m(3, 3);

}

template<typename T>
class InfinitePlane {

 public:

  /**
   * Create an infinite plane in point-normal form.
   */
  InfinitePlane(Vector3D<T> r0, const Vector3D<T> &n) :
      _r0{r0},
      _n{n.normalised()} {}

  /**
   * Create an infinite plane using canonical form:
   *      $A (x - x0) + B (y - y0) + C (z - z0) = 0$.
   * Note: the quantities A, B & C are subsequently normalised to a unit
   *       vector.
   *
   */
  InfinitePlane(T A, T B, T C, T x0, T y0, T z0) :
      _r0{x0, y0, z0},
      _n{A, B, C} {
    _n = normalised(_n);
  }

  [[nodiscard]] const Vector3D<T> &
  r0() const {
    return _r0;
  }

  [[nodiscard]] const Vector3D<T> &
  n() const {
    return _n;
  }

  [[nodiscard]] T
  A() const {
    return _n.x();
  }

  [[nodiscard]] T
  B() const {
    return _n.y();
  }
  [[nodiscard]] T
  C() const {
    return _n.z();
  }
  [[nodiscard]] T
  x0() const {
    return _r0.z();
  }
  [[nodiscard]] T
  x1() const {
    return _r0.y();
  }
  [[nodiscard]] T
  x2() const {
    return _r0.z();
  }
 private:

  Vector3D<T> _r0;
  Vector3D<T> _n;

};

template<typename T>
class Sphere {

 public:

  Sphere(Vector3D<T> p0, T r) : _p0{p0}, _r{r} {}

  Sphere(T x0, T y0, T z0, T r) : _p0{x0, y0, z0}, _r{r} {}

 private:

  Vector3D<T> _p0;
  T _r;

};

template<typename T>
void
sphere_plane_intersection(const Sphere<T> &sphere,
                          const InfinitePlane<T> &plane) {

}

} // namespace lgpl

#endif //LCGL__LCGL_HPP_

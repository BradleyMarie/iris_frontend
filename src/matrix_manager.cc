#include "src/matrix_manager.h"

#include <iostream>

namespace iris {

MatrixManager::MatrixManager() : m_active(ALL_TRANSFORMS) {}

const std::pair<Matrix, Matrix>& MatrixManager::GetCurrent() {
  m_current.first = m_cache.Lookup(m_current.first);
  m_current.second = m_cache.Lookup(m_current.second);
  return m_current;
}

void MatrixManager::Identity() { Set(Matrix()); }

void MatrixManager::Translate(float_t x, float_t y, float_t z) {
  Matrix translation;
  ISTATUS status =
      MatrixAllocateTranslation(x, y, z, translation.release_and_get_address());

  switch (status) {
    case ISTATUS_INVALID_ARGUMENT_00:
      std::cerr << "ERROR: Invalid x parameter to Translate " << x << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_01:
      std::cerr << "ERROR: Invalid y parameter to Translate " << x << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_02:
      std::cerr << "ERROR: Invalid z parameter to Translate " << x << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  Transform(translation);
}

void MatrixManager::Scale(float_t x, float_t y, float_t z) {
  Matrix scalar;
  ISTATUS status =
      MatrixAllocateScalar(x, y, z, scalar.release_and_get_address());

  switch (status) {
    case ISTATUS_INVALID_ARGUMENT_00:
      std::cerr << "ERROR: Invalid x parameter to Scale " << x << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_01:
      std::cerr << "ERROR: Invalid y parameter to Scale " << x << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_02:
      std::cerr << "ERROR: Invalid z parameter to Scale " << x << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  Transform(scalar);
}

void MatrixManager::Rotate(float_t theta, float_t x, float_t y, float_t z) {
  Matrix rotation;
  ISTATUS status = MatrixAllocateRotation(theta, x, y, z,
                                          rotation.release_and_get_address());

  switch (status) {
    case ISTATUS_INVALID_ARGUMENT_00:
      std::cerr << "ERROR: Invalid theta parameter to Rotate " << x
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_01:
      std::cerr << "ERROR: Invalid x parameter to Rotate " << x << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_02:
      std::cerr << "ERROR: Invalid y parameter to Rotate " << x << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_03:
      std::cerr << "ERROR: Invalid z parameter to Rotate " << x << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_COMBINATION_00:
      std::cerr
          << "ERROR: One of the x, y, or z arguments to Rotate must be non-zero"
          << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  Transform(rotation);
}

void MatrixManager::LookAt(float_t eye_x, float_t eye_y, float_t eye_z,
                           float_t look_x, float_t look_y, float_t look_z,
                           float_t up_x, float_t up_y, float_t up_z) {
  // TODO
}

void MatrixManager::CoordinateSystem(const std::string& name) {
  m_coordinate_systems[name] = m_current;
}

void MatrixManager::CoordSysTransform(const std::string& name) {
  auto result = m_coordinate_systems.find(name);
  if (result != m_coordinate_systems.end()) {
    std::cerr << "WARNING: No coordinate system with name '" << name
              << "' found" << std::endl;
  } else {
    m_current = result->second;
  }
}

void MatrixManager::Transform(float_t m00, float_t m01, float_t m02,
                              float_t m03, float_t m10, float_t m11,
                              float_t m12, float_t m13, float_t m20,
                              float_t m21, float_t m22, float_t m23,
                              float_t m30, float_t m31, float_t m32,
                              float_t m33) {
  Matrix transform;
  ISTATUS status =
      MatrixAllocate(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23,
                     m30, m31, m32, m33, transform.release_and_get_address());

  switch (status) {
    case ISTATUS_INVALID_ARGUMENT_00:
      std::cerr << "ERROR: Invalid m00 parameter to Transform " << m00
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_01:
      std::cerr << "ERROR: Invalid m01 parameter to Transform " << m01
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_02:
      std::cerr << "ERROR: Invalid m02 parameter to Transform " << m02
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_03:
      std::cerr << "ERROR: Invalid m03 parameter to Transform " << m03
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_04:
      std::cerr << "ERROR: Invalid m10 parameter to Transform " << m10
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_05:
      std::cerr << "ERROR: Invalid m11 parameter to Transform " << m11
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_06:
      std::cerr << "ERROR: Invalid m12 parameter to Transform " << m12
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_07:
      std::cerr << "ERROR: Invalid m13 parameter to Transform " << m13
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_08:
      std::cerr << "ERROR: Invalid m20 parameter to Transform " << m20
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_09:
      std::cerr << "ERROR: Invalid m21 parameter to Transform " << m21
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_10:
      std::cerr << "ERROR: Invalid m22 parameter to Transform " << m22
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_11:
      std::cerr << "ERROR: Invalid m23 parameter to Transform " << m23
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_12:
      std::cerr << "ERROR: Invalid m30 parameter to Transform " << m30
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_13:
      std::cerr << "ERROR: Invalid m31 parameter to Transform " << m31
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_14:
      std::cerr << "ERROR: Invalid m32 parameter to Transform " << m32
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_15:
      std::cerr << "ERROR: Invalid m33 parameter to Transform " << m33
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_ARITHMETIC_ERROR:
      std::cerr << "ERROR: Non-invertible matrix" << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  Set(transform);
}

void MatrixManager::ConcatTransform(float_t m00, float_t m01, float_t m02,
                                    float_t m03, float_t m10, float_t m11,
                                    float_t m12, float_t m13, float_t m20,
                                    float_t m21, float_t m22, float_t m23,
                                    float_t m30, float_t m31, float_t m32,
                                    float_t m33) {
  Matrix transform;
  ISTATUS status =
      MatrixAllocate(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23,
                     m30, m31, m32, m33, transform.release_and_get_address());

  switch (status) {
    case ISTATUS_INVALID_ARGUMENT_00:
      std::cerr << "ERROR: Invalid m00 parameter to Transform " << m00
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_01:
      std::cerr << "ERROR: Invalid m01 parameter to Transform " << m01
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_02:
      std::cerr << "ERROR: Invalid m02 parameter to Transform " << m02
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_03:
      std::cerr << "ERROR: Invalid m03 parameter to Transform " << m03
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_04:
      std::cerr << "ERROR: Invalid m10 parameter to Transform " << m10
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_05:
      std::cerr << "ERROR: Invalid m11 parameter to Transform " << m11
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_06:
      std::cerr << "ERROR: Invalid m12 parameter to Transform " << m12
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_07:
      std::cerr << "ERROR: Invalid m13 parameter to Transform " << m13
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_08:
      std::cerr << "ERROR: Invalid m20 parameter to Transform " << m20
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_09:
      std::cerr << "ERROR: Invalid m21 parameter to Transform " << m21
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_10:
      std::cerr << "ERROR: Invalid m22 parameter to Transform " << m22
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_11:
      std::cerr << "ERROR: Invalid m23 parameter to Transform " << m23
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_12:
      std::cerr << "ERROR: Invalid m30 parameter to Transform " << m30
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_13:
      std::cerr << "ERROR: Invalid m31 parameter to Transform " << m31
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_14:
      std::cerr << "ERROR: Invalid m32 parameter to Transform " << m32
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_INVALID_ARGUMENT_15:
      std::cerr << "ERROR: Invalid m33 parameter to Transform " << m33
                << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_ARITHMETIC_ERROR:
      std::cerr << "ERROR: Non-invertible matrix" << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  Transform(transform);
}

void MatrixManager::ActiveTransform(Active active) {
  assert(active == START_TRANSFORM || active == END_TRANSFORM ||
         active == ALL_TRANSFORMS);
  m_active = active;
}

void MatrixManager::Transform(Matrix m) {
  if (m_active & START_TRANSFORM) {
    Matrix old_value = m_current.first;
    ISTATUS status = MatrixAllocateProduct(
        old_value.get(), m.get(), m_current.first.release_and_get_address());

    switch (status) {
      case ISTATUS_ARITHMETIC_ERROR:
        std::cerr << "ERROR: Non-invertible matrix" << std::endl;
        exit(EXIT_FAILURE);
      case ISTATUS_ALLOCATION_FAILED:
        std::cerr << "ERROR: Allocation failed" << std::endl;
        exit(EXIT_FAILURE);
      default:
        assert(status == ISTATUS_SUCCESS);
    }
  }

  if (m_active & END_TRANSFORM) {
    Matrix old_value = m_current.second;
    ISTATUS status = MatrixAllocateProduct(
        old_value.get(), m.get(), m_current.second.release_and_get_address());

    switch (status) {
      case ISTATUS_ARITHMETIC_ERROR:
        std::cerr << "ERROR: Non-invertible matrix" << std::endl;
        exit(EXIT_FAILURE);
      case ISTATUS_ALLOCATION_FAILED:
        std::cerr << "ERROR: Allocation failed" << std::endl;
        exit(EXIT_FAILURE);
      default:
        assert(status == ISTATUS_SUCCESS);
    }
  }
}

void MatrixManager::Set(Matrix m) {
  if (m_active & START_TRANSFORM) {
    m_current.first = m;
  }

  if (m_active & END_TRANSFORM) {
    m_current.second = m;
  }
}

}  // namespace iris
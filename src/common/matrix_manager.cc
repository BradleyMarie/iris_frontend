#include "src/common/matrix_manager.h"

#include <iostream>

#include "absl/strings/str_join.h"

namespace iris {

MatrixManager::MatrixManager() : m_active(ALL_TRANSFORMS) {}

const std::pair<Matrix, Matrix>& MatrixManager::GetCurrent() {
  m_current.first = m_cache.Lookup(m_current.first);
  m_current.second = m_cache.Lookup(m_current.second);
  return m_current;
}

void MatrixManager::Identity() { Set(Matrix()); }

void MatrixManager::Translate(const std::array<FiniteFloatT, 3>& params) {
  Matrix translation;
  ISTATUS status = MatrixAllocateTranslation(
      params[0].Get(), params[1].Get(), params[2].Get(),
      translation.release_and_get_address());

  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  Transform(translation);
}

void MatrixManager::Scale(const std::array<FiniteNonZeroFloatT, 3>& params) {
  Matrix scalar;
  ISTATUS status =
      MatrixAllocateScalar(params[0].Get(), params[1].Get(), params[2].Get(),
                           scalar.release_and_get_address());

  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  Transform(scalar);
}

void MatrixManager::Rotate(const std::array<FiniteFloatT, 4>& params) {
  Matrix rotation;
  ISTATUS status = MatrixAllocateRotation(params[0].Get(), params[1].Get(),
                                          params[2].Get(), params[3].Get(),
                                          rotation.release_and_get_address());

  switch (status) {
    case ISTATUS_INVALID_ARGUMENT_COMBINATION_00:
      std::cerr << "ERROR: One of the x, y, or z parameters of Rotate must be "
                   "non-zero"
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

void MatrixManager::LookAt(const std::array<FiniteFloatT, 9>& params) {
  if (params[0].Get() == params[3].Get() &&
      params[1].Get() == params[4].Get() &&
      params[2].Get() == params[5].Get()) {
    std::cerr << "ERROR: Eye and look parameters of LookAt must be different "
                 "points"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  if (params[6].Get() == (float_t)0.0 && params[7].Get() == (float_t)0.0 &&
      params[8].Get() == (float_t)0.0) {
    std::cerr << "ERROR: Zero length up paramater to LookAt" << std::endl;
    exit(EXIT_FAILURE);
  }

  POINT3 pos = PointCreate(params[0].Get(), params[1].Get(), params[2].Get());
  POINT3 look = PointCreate(params[3].Get(), params[4].Get(), params[5].Get());

  VECTOR3 direction = PointSubtract(look, pos);
  direction = VectorNormalize(direction, nullptr, nullptr);

  VECTOR3 up = VectorCreate(params[6].Get(), params[7].Get(), params[8].Get());
  up = VectorNormalize(up, nullptr, nullptr);

  VECTOR3 right = VectorCrossProduct(up, direction);
  if (right.x == (float_t)0.0 && right.y == (float_t)0.0 &&
      right.z == (float_t)0.0) {
    std::cerr << "ERROR: Parallel look and up vectors for LookAt" << std::endl;
    exit(EXIT_FAILURE);
  }

  up = VectorCrossProduct(direction, right);

  Matrix inverse;
  ISTATUS status = MatrixAllocate(
      right.x, up.x, direction.x, pos.x, right.y, up.y, direction.y, pos.y,
      right.z, up.z, direction.z, pos.z, (float_t)0.0, (float_t)0.0,
      (float_t)0.0, (float_t)1.0, inverse.release_and_get_address());

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

  Matrix transform;
  *transform.release_and_get_address() = MatrixGetInverse(inverse.get());

  Transform(transform);
}

void MatrixManager::CoordinateSystem(absl::string_view name) {
  m_coordinate_systems[name] = m_current;
}

void MatrixManager::CoordSysTransform(absl::string_view name) {
  auto result = m_coordinate_systems.find(name);
  if (result != m_coordinate_systems.end()) {
    std::cerr << "WARNING: No coordinate system with name '" << name
              << "' found" << std::endl;
  } else {
    m_current = result->second;
  }
}

void MatrixManager::Transform(
    const std::array<FiniteFloatT, 16>& params,
    const std::array<std::string, 16> unparsed_params) {
  Matrix transform;
  ISTATUS status = MatrixAllocate(
      params[0].Get(), params[1].Get(), params[2].Get(), params[3].Get(),
      params[4].Get(), params[5].Get(), params[6].Get(), params[7].Get(),
      params[8].Get(), params[9].Get(), params[10].Get(), params[11].Get(),
      params[12].Get(), params[13].Get(), params[14].Get(), params[15].Get(),
      transform.release_and_get_address());

  switch (status) {
    case ISTATUS_ARITHMETIC_ERROR:
      std::cerr << "ERROR: Transform parameters were non-invertible: ["
                << absl::StrJoin(unparsed_params, ", ") << " ]" << std::endl;
      exit(EXIT_FAILURE);
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  Set(transform);
}

void MatrixManager::ConcatTransform(
    const std::array<FiniteFloatT, 16>& params,
    const std::array<std::string, 16> unparsed_params) {
  Matrix transform;
  ISTATUS status = MatrixAllocate(
      params[0].Get(), params[1].Get(), params[2].Get(), params[3].Get(),
      params[4].Get(), params[5].Get(), params[6].Get(), params[7].Get(),
      params[8].Get(), params[9].Get(), params[10].Get(), params[11].Get(),
      params[12].Get(), params[13].Get(), params[14].Get(), params[15].Get(),
      transform.release_and_get_address());

  switch (status) {
    case ISTATUS_ARITHMETIC_ERROR:
      std::cerr << "ERROR: ConcatTransform parameters were non-invertible: ["
                << absl::StrJoin(unparsed_params, ", ") << " ]" << std::endl;
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
        std::cerr << "ERROR: Non-invertible matrix product" << std::endl;
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
        std::cerr << "ERROR: Non-invertible matrix product" << std::endl;
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
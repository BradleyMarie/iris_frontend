#ifndef _SRC_MATRIX_MANAGER_
#define _SRC_MATRIX_MANAGER_

#include <array>
#include <set>
#include <string>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/string_view.h"
#include "src/finite_float_t.h"
#include "src/pointer_types.h"

namespace iris {

class MatrixManager {
 public:
  enum Active {
    START_TRANSFORM = 1,
    END_TRANSFORM = 2,
    ALL_TRANSFORMS = 3,
  };

  MatrixManager();
  const std::pair<Matrix, Matrix>& GetCurrent();
  void Identity();
  void Translate(const std::array<FiniteFloatT, 3>& params);
  void Scale(const std::array<FiniteNonZeroFloatT, 3>& params);
  void Rotate(const std::array<FiniteFloatT, 4>& params);
  void LookAt(const std::array<FiniteFloatT, 9>& params);
  void CoordinateSystem(absl::string_view name);
  void CoordSysTransform(absl::string_view name);
  void Transform(const std::array<FiniteFloatT, 16>& params,
                 const std::array<std::string, 16> unparsed_params);
  void ConcatTransform(const std::array<FiniteFloatT, 16>& params,
                       const std::array<std::string, 16> unparsed_params);
  void ActiveTransform(Active active);

  Active GetActive() const { return m_active; }
  void RestoreState(const std::pair<Matrix, Matrix>& transforms,
                    Active active) {
    m_current = transforms;
    m_active = active;
  }

 private:
  void Transform(Matrix m);
  void Set(Matrix m);

  class Cache {
   public:
    Matrix Lookup(Matrix matrix) {
      float_t contents[4][4];
      MatrixReadContents(matrix.get(), contents);
      if (contents[0][0] == (float_t)1.0 && contents[0][1] == (float_t)0.0 &&
          contents[0][2] == (float_t)0.0 && contents[0][3] == (float_t)0.0 &&
          contents[1][0] == (float_t)0.0 && contents[1][1] == (float_t)1.0 &&
          contents[1][2] == (float_t)0.0 && contents[1][3] == (float_t)0.0 &&
          contents[2][0] == (float_t)0.0 && contents[2][1] == (float_t)0.0 &&
          contents[2][2] == (float_t)1.0 && contents[2][3] == (float_t)0.0 &&
          contents[3][0] == (float_t)0.0 && contents[3][1] == (float_t)0.0 &&
          contents[3][2] == (float_t)0.0 && contents[3][3] == (float_t)1.0) {
        return Matrix();
      }

      auto it = m_transforms.find(matrix);
      if (it != m_transforms.end()) {
        return *it;
      }

      Matrix inverse;
      *inverse.release_and_get_address() = MatrixGetInverse(matrix.get());

      m_transforms.insert(matrix);
      m_transforms.insert(inverse);

      return matrix;
    }

   private:
    struct Comparator {
      bool operator()(const Matrix& lhs, const Matrix& rhs) const {
        float_t l_contents[4][4];
        MatrixReadContents(lhs.get(), l_contents);

        float_t r_contents[4][4];
        MatrixReadContents(rhs.get(), r_contents);

        size_t i, j;
        for (i = 0; i < 4; i++) {
          for (j = 0; j < 4; j++) {
            if (l_contents[i][j] != r_contents[i][j]) {
              break;
            }
          }
        }

        return l_contents[i][j] < r_contents[i][j];
      }
    };

    std::set<Matrix, Comparator> m_transforms;
  };

  Active m_active;
  absl::flat_hash_map<std::string, std::pair<Matrix, Matrix>>
      m_coordinate_systems;
  std::pair<Matrix, Matrix> m_current;
  Cache m_cache;
};

}  // namespace iris

#endif  // _SRC_MATRIX_MANAGER_
#ifndef _SRC_MATRIX_MANAGER_
#define _SRC_MATRIX_MANAGER_

#include <map>
#include <set>

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
  void Translate(float_t x, float_t y, float_t z);
  void Scale(float_t x, float_t y, float_t z);
  void Rotate(float_t theta, float_t x, float_t y, float_t z);
  void LookAt(float_t eye_x, float_t eye_y, float_t eye_z, float_t look_x,
              float_t look_y, float_t look_z, float_t up_x, float_t up_y,
              float_t up_z);
  void CoordinateSystem(const std::string& name);
  void CoordSysTransform(const std::string& name);
  void Transform(float_t m00, float_t m01, float_t m02, float_t m03,
                 float_t m10, float_t m11, float_t m12, float_t m13,
                 float_t m20, float_t m21, float_t m22, float_t m23,
                 float_t m30, float_t m31, float_t m32, float_t m33);
  void ConcatTransform(float_t m00, float_t m01, float_t m02, float_t m03,
                       float_t m10, float_t m11, float_t m12, float_t m13,
                       float_t m20, float_t m21, float_t m22, float_t m23,
                       float_t m30, float_t m31, float_t m32, float_t m33);
  void ActiveTransform(Active active);

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
  std::map<std::string, std::pair<Matrix, Matrix>> m_coordinate_systems;
  std::pair<Matrix, Matrix> m_current;
  Cache m_cache;
};

}  // namespace iris

#endif  // _SRC_MATRIX_MANAGER_
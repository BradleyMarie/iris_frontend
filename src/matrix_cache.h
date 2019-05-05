#include <set>

#include "src/pointer_types.h"

namespace iris {

class MatrixCache {
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
      return Matrix(nullptr);
    }

    return *m_transforms.insert(matrix).first;
  }

  void clear() { m_transforms.clear(); }

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

}  // namespace iris
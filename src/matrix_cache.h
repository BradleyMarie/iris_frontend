#include <set>

#include "src/pointer_types.h"

namespace iris {

class MatrixCache {
 public:
  Matrix Lookup(Matrix matrix) { return *m_transforms.insert(matrix).first; }
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
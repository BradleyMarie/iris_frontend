#ifndef _SRC_COMMON_OBJECT_MANAGER_
#define _SRC_COMMON_OBJECT_MANAGER_

#include <vector>

#include "absl/container/flat_hash_map.h"
#include "src/common/pointer_types.h"
#include "src/common/tokenizer.h"

namespace iris {

class ObjectManager {
 public:
  ObjectManager() : m_current(nullptr) {}
  ObjectManager(const ObjectManager&) = delete;
  ObjectManager& operator=(const ObjectManager&) = delete;
  ~ObjectManager();

  void ObjectBegin(Tokenizer& tokenizer);
  void ObjectInstance(Tokenizer& tokenizer, const Matrix& matrix);
  void ObjectEnd();

  void AddShape(const Shape& shape, const Matrix& matrix,
                const EmissiveMaterial& front, const EmissiveMaterial& back);

  Scene AllocateScene();

 private:
  std::vector<std::tuple<Shape>>* m_current;
  absl::flat_hash_map<std::string, std::vector<std::tuple<Shape>>>
      m_instanced_objects;

  std::vector<PMATRIX> m_scene_transforms;
  std::vector<PSHAPE> m_scene_shapes;
};

}  // namespace iris

#endif  // _SRC_COMMON_OBJECT_MANAGER_
#ifndef _SRC_COMMON_OBJECT_MANAGER_
#define _SRC_COMMON_OBJECT_MANAGER_

#include <tuple>
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

  void AddShape(const Shape& shape, const Matrix& matrix);
  void AddAreaLight(const Shape& shape, const Matrix& matrix,
                    const EmissiveMaterial& material, uint32_t face_index);

  std::pair<Scene, std::vector<Light>> AllocateScene();

 private:
  std::pair<std::vector<Shape>,
            std::vector<std::tuple<Shape, EmissiveMaterial, uint32_t>>>*
      m_current;
  absl::flat_hash_map<
      std::string,
      std::pair<std::vector<Shape>,
                std::vector<std::tuple<Shape, EmissiveMaterial, uint32_t>>>>
      m_instanced_objects;

  std::vector<Light> m_scene_lights;
  std::vector<PMATRIX> m_scene_transforms;
  std::vector<PSHAPE> m_scene_shapes;
};

}  // namespace iris

#endif  // _SRC_COMMON_OBJECT_MANAGER_
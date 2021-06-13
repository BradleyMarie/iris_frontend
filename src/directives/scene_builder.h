#ifndef _SRC_DIRECTIVES_SCENE_BUILDER_
#define _SRC_DIRECTIVES_SCENE_BUILDER_

#include <tuple>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "src/common/directive.h"
#include "src/common/pointer_types.h"

namespace iris {

class SceneBuilder {
 public:
  SceneBuilder() : m_build_instanced_object(false) {}
  SceneBuilder(const SceneBuilder&) = delete;
  SceneBuilder& operator=(const SceneBuilder&) = delete;
  ~SceneBuilder();

  void ObjectBegin(Directive& directive);
  void ObjectInstance(Directive& directive, const Matrix& matrix);
  void ObjectEnd(Directive& directive);

  void AddShape(const Shape& shape, const Matrix& matrix);
  void AddAreaLight(const Shape& shape, const Matrix& matrix,
                    const EmissiveMaterial& material, uint32_t face_index);
  void AddLight(const Light& light,
                const EnvironmentalLight& environmental_light);

  std::pair<Scene, std::vector<Light>> Build();

 private:
  std::vector<Shape> m_instanced_object_shapes;
  std::vector<std::tuple<Shape, EmissiveMaterial, uint32_t>>
      m_instanced_object_area_lights;
  std::string m_instanced_object_name;
  bool m_build_instanced_object;

  absl::flat_hash_map<
      std::string,
      std::pair<Shape,
                std::vector<std::tuple<Shape, EmissiveMaterial, uint32_t>>>>
      m_instanced_objects;

  std::vector<Light> m_scene_lights;
  std::vector<std::tuple<Light, EnvironmentalLight>> m_environmental_lights;
  std::vector<PMATRIX> m_scene_transforms;
  std::vector<PSHAPE> m_scene_shapes;
};

}  // namespace iris

#endif  // _SRC_DIRECTIVES_SCENE_BUILDER_
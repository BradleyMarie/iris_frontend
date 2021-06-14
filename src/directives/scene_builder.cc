#include "src/directives/scene_builder.h"

#include <iostream>

#include "iris_physx_toolkit/scenes/kd_tree.h"
#include "iris_physx_toolkit/aggregate_environmental_light.h"
#include "src/common/error.h"

namespace iris {

SceneBuilder::~SceneBuilder() {
  for (PMATRIX matrix : m_scene_transforms) {
    MatrixRelease(matrix);
  }

  for (PSHAPE shape : m_scene_shapes) {
    ShapeRelease(shape);
  }
}

void SceneBuilder::ObjectBegin(Directive& directive) {
  if (m_build_instanced_object) {
    std::cerr << "ERROR: Mismatched ObjectBegin and ObjectEnd directives"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  m_instanced_object_name = directive.SingleQuotedString("name");
  m_instanced_objects.erase(m_instanced_object_name);
  m_build_instanced_object = true;
}

void SceneBuilder::ObjectInstance(Directive& directive, const Matrix& matrix) {
  if (m_build_instanced_object) {
    std::cerr << "ERROR: Invalid directive between ObjectBegin and "
                 "ObjectEnd: ObjectInstance"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  auto name = directive.SingleQuotedString("name");

  auto iter = m_instanced_objects.find(name);
  if (iter == m_instanced_objects.end()) {
    std::cerr << "ERROR: ObjectInstance not defined: " << name << std::endl;
    exit(EXIT_FAILURE);
  }

  if (iter->second.first.get()) {
    AddShape(iter->second.first, matrix);
  }

  for (const auto& entry : iter->second.second) {
    AddAreaLight(std::get<0>(entry), matrix, std::get<1>(entry),
                 std::get<2>(entry));
  }
}

void SceneBuilder::ObjectEnd(Directive& directive) {
  if (!m_build_instanced_object) {
    std::cerr << "ERROR: Mismatched ObjectBegin and ObjectEnd directives"
              << std::endl;
    exit(EXIT_FAILURE);
  }
  directive.Empty();

  std::vector<PSHAPE> shapes;
  for (const auto& shape : m_instanced_object_shapes) {
    shapes.push_back(shape.get());
  }

  Shape shape;
  if (shapes.size() == 1) {
    shape = m_instanced_object_shapes[0];
  } else {
    ISTATUS status = KdTreeAggregateAllocate(shapes.data(), shapes.size(),
                                             shape.release_and_get_address());
    SuccessOrOOM(status);
  }

  auto& entry = m_instanced_objects[m_instanced_object_name];
  entry.first = shape;
  entry.second = std::move(m_instanced_object_area_lights);

  m_instanced_object_shapes.clear();
  m_instanced_object_area_lights.clear();
  m_instanced_object_name.clear();
  m_build_instanced_object = false;
}

void SceneBuilder::AddShape(const Shape& shape, const Matrix& matrix) {
  if (m_build_instanced_object) {
    if (matrix.get()) {
      std::cerr << "ERROR: Transformations not supported in instanced objects"
                << std::endl;
      exit(EXIT_FAILURE);
    }

    m_instanced_object_shapes.push_back(shape);
  } else {
    m_scene_shapes.push_back(shape.get());
    ShapeRetain(m_scene_shapes.back());
    m_scene_transforms.push_back(matrix.get());
    MatrixRetain(m_scene_transforms.back());
  }
}

void SceneBuilder::AddAreaLight(const Shape& shape, const Matrix& matrix,
                                const EmissiveMaterial& material,
                                uint32_t face_index) {
  if (m_build_instanced_object) {
    if (matrix.get()) {
      std::cerr << "ERROR: Transformations not supported in instanced objects"
                << std::endl;
      exit(EXIT_FAILURE);
    }

    m_instanced_object_area_lights.push_back(
        std::make_tuple(shape, material, face_index));
  } else {
    Light light;
    ISTATUS status = AreaLightAllocate(shape.get(), face_index, matrix.get(),
                                       light.release_and_get_address());
    SuccessOrOOM(status);
    m_scene_lights.push_back(light);
  }
}

void SceneBuilder::AddLight(const Light& light,
                            const EnvironmentalLight& environmental_light) {
  if (environmental_light.get()) {
    m_environmental_lights.push_back(
        std::make_tuple(light, environmental_light));
  } else {
    m_scene_lights.push_back(light);
  }
}

std::pair<Scene, std::vector<Light>> SceneBuilder::Build() {
  assert(m_scene_shapes.size() == m_scene_transforms.size());

  std::vector<Light> result_lights = m_scene_lights;

  EnvironmentalLight environmental_light;
  if (m_environmental_lights.size() == 1) {
    environmental_light = std::get<1>(m_environmental_lights[0]);
    result_lights.push_back(std::get<0>(m_environmental_lights[0]));
  } else if (m_environmental_lights.size() != 0) {
    std::vector<PENVIRONMENTAL_LIGHT> environmental_lights;
    for (const auto& entry : m_environmental_lights) {
      environmental_lights.push_back(std::get<1>(entry).get());
    }

    Light environmental_light_as_light;
    ISTATUS status = AggregateEnvironmentalLightAllocate(
        environmental_lights.data(), environmental_lights.size(),
        environmental_light.release_and_get_address(),
        environmental_light_as_light.release_and_get_address());
    SuccessOrOOM(status);

    result_lights.push_back(environmental_light_as_light);
  }

  Scene result;
  ISTATUS status = KdTreeSceneAllocate(
      m_scene_shapes.data(), m_scene_transforms.data(), nullptr,
      m_scene_shapes.size(), environmental_light.get(),
      result.release_and_get_address());
  SuccessOrOOM(status);

  return std::make_pair(result, result_lights);
}

}  // namespace iris
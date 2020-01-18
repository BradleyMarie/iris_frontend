#include "src/common/object_manager.h"

#include <iostream>

#include "iris_physx_toolkit/kd_tree_scene.h"
#include "src/common/error.h"
#include "src/common/quoted_string.h"

namespace iris {
namespace {

absl::string_view ParseNextQuotedString(const char* base_type_name,
                                        Tokenizer& tokenizer,
                                        absl::string_view element_name) {
  auto token = tokenizer.Next();
  if (!token) {
    std::cerr << "ERROR: " << base_type_name << " " << element_name
              << " not specified" << std::endl;
    exit(EXIT_FAILURE);
  }

  auto unquoted = UnquoteToken(*token);
  if (!unquoted) {
    std::cerr << "ERROR: Invalid " << base_type_name << " " << element_name
              << " specified: " << *token << std::endl;
    exit(EXIT_FAILURE);
  }

  return *unquoted;
}

}  // namespace

ObjectManager::~ObjectManager() {
  for (PMATRIX matrix : m_scene_transforms) {
    MatrixRelease(matrix);
  }

  for (PSHAPE shape : m_scene_shapes) {
    ShapeRelease(shape);
  }
}

void ObjectManager::ObjectBegin(Tokenizer& tokenizer) {
  if (m_current) {
    std::cerr << "ERROR: Mismatched ObjectBegin and ObjectEnd directives"
              << std::endl;
    exit(EXIT_FAILURE);
  }
  std::string name(ParseNextQuotedString("ObjectBegin", tokenizer, "name"));
  m_current = &m_instanced_objects[name];
  m_current->clear();
}

void ObjectManager::ObjectInstance(Tokenizer& tokenizer, const Matrix& matrix) {
  if (m_current) {
    std::cerr << "ERROR: Invalid directive between ObjectBegin and "
                 "ObjectEnd: ObjectInstance"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string name(ParseNextQuotedString("ObjectInstance", tokenizer, "name"));
  auto iter = m_instanced_objects.find(name);
  if (iter == m_instanced_objects.end()) {
    std::cerr << "ERROR: ObjectInstance not defined: " << name << std::endl;
    exit(EXIT_FAILURE);
  }

  for (const auto& entry : iter->second) {
    AddShape(std::get<0>(entry), matrix, EmissiveMaterial(),
             EmissiveMaterial());
  }
}

void ObjectManager::ObjectEnd() {
  if (!m_current) {
    std::cerr << "ERROR: Mismatched ObjectBegin and ObjectEnd directives"
              << std::endl;
    exit(EXIT_FAILURE);
  }
  m_current = nullptr;
}

void ObjectManager::AddShape(const Shape& shape, const Matrix& matrix,
                             const EmissiveMaterial& front,
                             const EmissiveMaterial& back) {
  if (m_current) {
    if (matrix.get()) {
      std::cerr << "ERROR: Transformations not supported in instanced objects"
                << std::endl;
      exit(EXIT_FAILURE);
    }

    if (front.get() || back.get()) {
      std::cerr << "ERROR: Area lights not supported in instanced objects"
                << std::endl;
      exit(EXIT_FAILURE);
    }

    m_current->push_back(std::make_tuple(shape));
  } else {
    m_scene_shapes.push_back(shape.get());
    ShapeRetain(m_scene_shapes.back());
    m_scene_transforms.push_back(matrix.get());
    MatrixRetain(m_scene_transforms.back());
  }
}

Scene ObjectManager::AllocateScene() {
  assert(m_scene_shapes.size() == m_scene_transforms.size());

  std::unique_ptr<bool[]> premultiplied(new bool[m_scene_shapes.size()]());
  Scene result;
  ISTATUS status = KdTreeSceneAllocate(
      m_scene_shapes.data(), m_scene_transforms.data(), premultiplied.get(),
      m_scene_shapes.size(), result.release_and_get_address());
  SuccessOrOOM(status);

  return result;
}

}  // namespace iris
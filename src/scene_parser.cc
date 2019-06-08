#include <iostream>
#include <stack>

#include "iris_physx_toolkit/kd_tree_scene.h"
#include "src/matrix_parser.h"
#include "src/scene_parser.h"

namespace iris {
namespace {

class GraphicsStateManager {
 public:
  GraphicsStateManager() { m_shader_state.push(ShaderState()); }

  void TransformBegin(MatrixManager& matrix_manager);
  void TransformEnd(MatrixManager& matrix_manager);

  void AttributeBegin(MatrixManager& matrix_manager);
  void AttributeEnd(MatrixManager& matrix_manager);

 private:
  enum Face {
    FRONT = 1,
    BACK = 2,
    BOTH = 3,
  };

  struct AreaLightState {
    Spectrum spectrum;
    Face face;
  };

  struct ShaderState {
    absl::optional<AreaLightState> area_light;
    absl::optional<Material> material;
  };

  enum PushReason {
    TRANSFORM = 1,
    ATTRIBUTE = 2,
  };

  struct TransformState {
    std::pair<Matrix, Matrix> transforms;
    MatrixManager::Active active_transforms;
    PushReason push_reason;
  };

  std::stack<ShaderState> m_shader_state;
  std::stack<TransformState> m_transform_state;
  std::map<std::string, Material> m_named_materials;
  std::map<std::string, std::vector<Shape>> m_named_objects;
};

void GraphicsStateManager::TransformBegin(MatrixManager& matrix_manager) {
  m_transform_state.push(
      {matrix_manager.GetCurrent(), matrix_manager.GetActive(), TRANSFORM});
}

void GraphicsStateManager::TransformEnd(MatrixManager& matrix_manager) {
  if (m_transform_state.empty() ||
      m_transform_state.top().push_reason != TRANSFORM) {
    std::cerr << "ERROR: Mismatched TransformBegin and TransformEnd directives"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  matrix_manager.RestoreState(m_transform_state.top().transforms,
                              m_transform_state.top().active_transforms);

  m_transform_state.pop();
}

void GraphicsStateManager::AttributeBegin(MatrixManager& matrix_manager) {
  m_transform_state.push(
      {matrix_manager.GetCurrent(), matrix_manager.GetActive(), ATTRIBUTE});
  m_shader_state.push(m_shader_state.top());
}

void GraphicsStateManager::AttributeEnd(MatrixManager& matrix_manager) {
  if (m_transform_state.empty() ||
      m_transform_state.top().push_reason != ATTRIBUTE) {
  }

  matrix_manager.RestoreState(m_transform_state.top().transforms,
                              m_transform_state.top().active_transforms);

  m_transform_state.pop();
  m_shader_state.pop();

  if (m_shader_state.empty()) {
    std::cerr << "ERROR: Mismatched AttributeBegin and AttributeEnd directives"
              << std::endl;
    exit(EXIT_FAILURE);
  }
}

Scene CreateScene(std::vector<Shape>& shapes, std::vector<Matrix>& transforms) {
  assert(shapes.size() == transforms.size());
  std::vector<PSHAPE> shape_pointers;
  std::vector<PMATRIX> matrix_pointers;
  std::unique_ptr<bool[]> premultiplied(new bool[shapes.size()]);

  for (size_t i = 0; i < shapes.size(); i++) {
    shape_pointers.push_back(shapes[i].get());
    matrix_pointers.push_back(transforms[i].get());
    premultiplied[i] = false;
  }

  Scene result;
  ISTATUS status = KdTreeSceneAllocate(
      shape_pointers.data(), matrix_pointers.data(), premultiplied.get(),
      shapes.size(), result.release_and_get_address());
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      std::cerr << "ERROR: Allocation failed" << std::endl;
      exit(EXIT_FAILURE);
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  return result;
}

}  // namespace

std::pair<Scene, std::vector<Light>> ParseScene(
    Tokenizer& tokenizer, MatrixManager& matrix_manager,
    ColorIntegrator& color_integrator) {
  matrix_manager.ActiveTransform(MatrixManager::ALL_TRANSFORMS);
  matrix_manager.Identity();

  std::vector<Shape> shapes;
  std::vector<Matrix> transforms;
  std::vector<Light> lights;
  GraphicsStateManager graphics_state;
  for (auto token = tokenizer.Next(); token; token = tokenizer.Next()) {
    if (token == "WorldEnd") {
      return std::make_pair(CreateScene(shapes, transforms), std::move(lights));
    }

    if (TryParseMatrix(*token, tokenizer, matrix_manager)) {
      continue;
    }

    if (token == "AttributeBegin") {
      graphics_state.AttributeBegin(matrix_manager);
      continue;
    }

    if (token == "AttributeEnd") {
      graphics_state.AttributeEnd(matrix_manager);
      continue;
    }

    if (token == "TransformBegin") {
      graphics_state.TransformBegin(matrix_manager);
      continue;
    }

    if (token == "TransformEnd") {
      graphics_state.TransformEnd(matrix_manager);
      continue;
    }

    if (token == "MakeNamedMedium") {
      std::cerr << "ERROR: Invalid directive before WorldBegin: " << *token
                << std::endl;
      exit(EXIT_FAILURE);
      continue;
    }

    if (token == "MediumInterface") {
      std::cerr << "ERROR: Invalid directive before WorldBegin: " << *token
                << std::endl;
      exit(EXIT_FAILURE);
      continue;
    }

    if (token == "TransformTimes") {
      std::cerr << "ERROR: Invalid directive before WorldBegin: " << *token
                << std::endl;
      exit(EXIT_FAILURE);
      continue;
    }

    std::cerr << "ERROR: Invalid directive before WorldEnd: " << *token
              << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cerr << "ERROR: Missing WorldEnd directive" << std::endl;
  exit(EXIT_FAILURE);
}

}  // namespace iris
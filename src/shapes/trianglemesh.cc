#include "src/shapes/trianglemesh.h"

#include <iostream>

#include "iris_physx_toolkit/triangle_mesh.h"
#include "src/common/error.h"
#include "src/common/ostream.h"
#include "src/param_matchers/list.h"
#include "src/param_matchers/matcher.h"

namespace iris {
namespace {

typedef ListValueMatcher<Point3Parameter, POINT3, 3, 1>
    TriangleMeshPointListMatcher;

typedef ListValueMatcher<IntParameter, int, 3, 3> TriangleMeshIndexListMatcher;

static const std::vector<POINT3> kTriangleMeshDefaultPoints;
static const std::vector<int> kTriangleMeshDefaultIndices;

}  // namespace

ShapeResult ParseTriangleMesh(const char* base_type_name, const char* type_name,
                              Tokenizer& tokenizer,
                              const Material& front_material,
                              const Material& back_material,
                              const EmissiveMaterial& front_emissive_material,
                              const EmissiveMaterial& back_emissive_material) {
  TriangleMeshPointListMatcher points(base_type_name, type_name, "P", true,
                                      kTriangleMeshDefaultPoints);
  TriangleMeshIndexListMatcher int_indices(base_type_name, type_name, "indices",
                                           true, kTriangleMeshDefaultIndices);
  MatchParameters<2>(base_type_name, type_name, tokenizer,
                     {&points, &int_indices});

  std::vector<size_t> indices;
  for (const auto& entry : int_indices.Get()) {
    static_assert(INT32_MAX < SIZE_MAX);
    if (entry < 0 || points.Get().size() <= (size_t)entry) {
      std::cerr << "ERROR: Out of range value for " << type_name << " "
                << base_type_name << " parameter: indices" << std::endl;
      exit(EXIT_FAILURE);
    }
    indices.push_back(entry);
  }

  // TODO: Check for nonsensical indices

  std::vector<Shape> shapes(indices.size() / 3);
  size_t triangles_allocated;
  ISTATUS status = TriangleMeshAllocate(
      points.Get().data(), nullptr, nullptr, points.Get().size(),
      reinterpret_cast<const size_t(*)[3]>(indices.data()), indices.size() / 3,
      front_material.get(), back_material.get(), front_emissive_material.get(),
      back_emissive_material.get(), reinterpret_cast<PSHAPE*>(shapes.data()),
      &triangles_allocated);
  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      ReportOOM();
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  if (triangles_allocated != indices.size() / 3) {
    std::cerr << "WARNING: TriangleMesh contained degenerate triangles that "
                 "were ignored."
              << std::endl;
  }

  std::vector<Light> lights;
  if (!front_emissive_material.get() && !back_emissive_material.get()) {
    return std::make_pair(std::move(shapes), std::move(lights));
  }

  for (size_t i = 0; i < triangles_allocated; i++) {
    if (front_emissive_material.get()) {
      Light light;
      status = AreaLightAllocate(shapes[i].get(), TRIANGLE_MESH_FRONT_FACE,
                                 light.release_and_get_address());
      SuccessOrOOM(status);
      lights.push_back(light);
    }

    if (back_emissive_material.get()) {
      Light light;
      status = AreaLightAllocate(shapes[i].get(), TRIANGLE_MESH_BACK_FACE,
                                 light.release_and_get_address());
      SuccessOrOOM(status);
      lights.push_back(light);
    }
  }

  return std::make_pair(std::move(shapes), std::move(lights));
}

}  // namespace iris
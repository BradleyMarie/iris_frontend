#include "src/shapes/trianglemesh.h"

#include "iris_physx_toolkit/triangle.h"
#include "src/ostream.h"
#include "src/param_matcher.h"

#include <iostream>

namespace iris {
namespace {

typedef ListValueMatcher<Point3Parameter, POINT3, 3, 1>
    TriangleMeshPointListMatcher;

typedef PreBoundedListValueMatcher<IntParameter, size_t, int, 0, INT_MAX, 3, 3>
    TriangleMeshIndexListMatcher;

static const std::vector<POINT3> kTriangleMeshDefaultPoints;
static const std::vector<size_t> kTriangleMeshDefaultIndices;

}  // namespace

ShapeResult ParseTriangleMesh(const char* base_type_name, const char* type_name,
                              Tokenizer& tokenizer,
                              const Material& front_material,
                              const Material& back_material,
                              const EmissiveMaterial& front_emissive_material,
                              const EmissiveMaterial& back_emissive_material) {
  TriangleMeshPointListMatcher points(base_type_name, type_name, "P",
                                      kTriangleMeshDefaultPoints);
  TriangleMeshIndexListMatcher indices(base_type_name, type_name, "indices",
                                       kTriangleMeshDefaultIndices);
  ParseAllParameter<2>(base_type_name, type_name, tokenizer,
                       {&points, &indices});

  if (points.Get().empty()) {
    std::cerr << "ERROR: Missing required " << type_name << " "
              << base_type_name << " parameter: P" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (indices.Get().empty()) {
    std::cerr << "ERROR: Missing required " << type_name << " "
              << base_type_name << " parameter: indices" << std::endl;
    exit(EXIT_FAILURE);
  }

  for (const auto& entry : indices.Get()) {
    if (points.Get().size() <= entry) {
      std::cerr << "ERROR: Out of range value for " << type_name << " "
                << base_type_name << " parameter: indices" << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  // TODO: Check for nonsensical indices

  std::vector<Shape> shapes;
  std::vector<Light> lights;
  for (size_t i = 0; i < indices.Get().size(); i += 3) {
    POINT3 p0 = points.Get()[indices.Get()[i]];
    POINT3 p1 = points.Get()[indices.Get()[i + 1]];
    POINT3 p2 = points.Get()[indices.Get()[i + 2]];

    Shape triangle;
    ISTATUS status = EmissiveTriangleAllocate(
        p0, p1, p2, front_material.get(), back_material.get(),
        front_emissive_material.get(), back_emissive_material.get(),
        triangle.release_and_get_address());
    switch (status) {
      case ISTATUS_ALLOCATION_FAILED:
        std::cerr << "ERROR: Allocation failed" << std::endl;
        exit(EXIT_FAILURE);
      default:
        assert(status == ISTATUS_SUCCESS);
    }

    if (!triangle.get()) {
      std::cerr << "WARNING: Degenerate triangle skipped: " << p0 << ", " << p1
                << ", " << p2 << std::endl;
      continue;
    }

    if (front_emissive_material.get()) {
      Light light;
      status = AreaLightAllocate(triangle.get(), TRIANGLE_FRONT_FACE,
                                 light.release_and_get_address());
      switch (status) {
        case ISTATUS_ALLOCATION_FAILED:
          std::cerr << "ERROR: Allocation failed" << std::endl;
          exit(EXIT_FAILURE);
        default:
          assert(status == ISTATUS_SUCCESS);
      }

      lights.push_back(light);
    }

    if (back_emissive_material.get()) {
      Light light;
      status = AreaLightAllocate(triangle.get(), TRIANGLE_BACK_FACE,
                                 light.release_and_get_address());
      switch (status) {
        case ISTATUS_ALLOCATION_FAILED:
          std::cerr << "ERROR: Allocation failed" << std::endl;
          exit(EXIT_FAILURE);
        default:
          assert(status == ISTATUS_SUCCESS);
      }

      lights.push_back(light);
    }

    shapes.push_back(triangle);
  }

  return std::make_pair(std::move(shapes), std::move(lights));
}

}  // namespace iris
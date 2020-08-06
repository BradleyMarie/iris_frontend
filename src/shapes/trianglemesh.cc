#include "src/shapes/trianglemesh.h"

#include <iostream>

#include "iris_physx_toolkit/triangle_mesh.h"
#include "src/common/error.h"
#include "src/common/ostream.h"
#include "src/param_matchers/list.h"

namespace iris {
namespace {

typedef ListValueMatcher<Point3Parameter, POINT3, 3, 1>
    TriangleMeshPointListMatcher;

typedef ListValueMatcher<IntParameter, int, 3, 3> TriangleMeshIndexListMatcher;

static const std::vector<POINT3> kTriangleMeshDefaultPoints;
static const std::vector<int> kTriangleMeshDefaultIndices;

}  // namespace

ShapeResult ParseTriangleMesh(Parameters& parameters,
                              const Matrix& model_to_world,
                              MaterialManager& material_manager,
                              const NamedTextureManager& named_texture_manager,
                              NormalMapManager& normal_map_manager,
                              TextureManager& texture_manager,
                              SpectrumManager& spectrum_manager,
                              const MaterialResult& material_result,
                              const EmissiveMaterial& front_emissive_material,
                              const EmissiveMaterial& back_emissive_material) {
  TriangleMeshPointListMatcher points("P", true, kTriangleMeshDefaultPoints);
  TriangleMeshIndexListMatcher int_indices("indices", true,
                                           kTriangleMeshDefaultIndices);
  auto unused_parameters = parameters.MatchAllowUnused(points, int_indices);
  auto material = material_result(unused_parameters, material_manager,
                                  named_texture_manager, normal_map_manager,
                                  texture_manager, spectrum_manager);

  std::vector<size_t> indices;
  for (const auto& entry : int_indices.Get()) {
    static_assert(INT32_MAX < SIZE_MAX);
    if (entry < 0 || points.Get().size() <= (size_t)entry) {
      std::cerr << "ERROR: Out of range value for " << unused_parameters.Name()
                << " parameter: indices" << std::endl;
      exit(EXIT_FAILURE);
    }
    indices.push_back(entry);
  }

  // TODO: Check for nonsensical indices

  for (auto& point : points.GetMutable()) {
    point = PointMatrixMultiply(model_to_world.get(), point);
  }

  std::vector<Shape> shapes(indices.size() / 3);
  size_t triangles_allocated;
  ISTATUS status = TriangleMeshAllocate(
      points.Get().data(), nullptr, points.Get().size(),
      reinterpret_cast<const size_t(*)[3]>(indices.data()), indices.size() / 3,
      nullptr, nullptr, material.first.get(), material.first.get(),
      front_emissive_material.get(), back_emissive_material.get(),
      reinterpret_cast<PSHAPE*>(shapes.data()), &triangles_allocated);
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
    shapes.resize(triangles_allocated);
  }

  std::vector<std::tuple<Shape, EmissiveMaterial, uint32_t>> emissive_faces;
  if (!front_emissive_material.get() && !back_emissive_material.get()) {
    return std::make_tuple(std::move(shapes), std::move(emissive_faces),
                           ShapeCoordinateSystem::World);
  }

  for (size_t i = 0; i < triangles_allocated; i++) {
    if (front_emissive_material.get()) {
      emissive_faces.push_back(std::make_tuple(
          shapes[i], front_emissive_material, TRIANGLE_MESH_FRONT_FACE));
    }

    if (back_emissive_material.get()) {
      emissive_faces.push_back(std::make_tuple(
          shapes[i], front_emissive_material, TRIANGLE_MESH_BACK_FACE));
    }
  }

  return std::make_tuple(std::move(shapes), std::move(emissive_faces),
                         ShapeCoordinateSystem::World);
}

}  // namespace iris
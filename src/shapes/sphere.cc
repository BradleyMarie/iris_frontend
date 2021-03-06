#include "src/shapes/sphere.h"

#include "iris_physx_toolkit/shapes/sphere.h"
#include "src/common/error.h"
#include "src/common/ostream.h"
#include "src/param_matchers/float_single.h"

namespace iris {
namespace {

static const POINT3 kSphereOrigin = {(float_t)0.0, (float_t)0.0, (float_t)0.0};
static const float_t kSphereDefaultRadius = (float_t)1.0;

bool IsUniformPositiveScaleAndTranslateOnly(const Matrix& matrix) {
  float_t contents[4][4];
  MatrixReadContents(matrix.get(), contents);

  bool result =
      contents[0][0] == contents[1][1] && contents[0][0] == contents[2][2] &&
      (float_t)0.0 < contents[0][0] && (float_t)0.0 < contents[3][3] &&
      (float_t)0.0 == contents[0][1] && (float_t)0.0 == contents[0][2] &&
      (float_t)0.0 == contents[1][0] && (float_t)0.0 == contents[1][2] &&
      (float_t)0.0 == contents[2][0] && (float_t)0.0 == contents[2][1] &&
      (float_t)0.0 == contents[3][0] && (float_t)0.0 == contents[3][1] &&
      (float_t)0.0 == contents[3][2];

  return result;
}

}  // namespace

ShapeResult ParseSphere(Parameters& parameters, const Matrix& model_to_world,
                        MaterialManager& material_manager,
                        const NamedTextureManager& named_texture_manager,
                        NormalMapManager& normal_map_manager,
                        TextureManager& texture_manager,
                        SpectrumManager& spectrum_manager,
                        const MaterialResult& material_result,
                        const EmissiveMaterial& front_emissive_material,
                        const EmissiveMaterial& back_emissive_material) {
  SingleFloatMatcher radius("radius", false, false, (float_t)0.0,
                            std::numeric_limits<float_t>::infinity(),
                            kSphereDefaultRadius);
  auto unused_parameters = parameters.MatchAllowUnused(radius);
  auto material = material_result(unused_parameters, material_manager,
                                  named_texture_manager, normal_map_manager,
                                  texture_manager, spectrum_manager);

  ShapeCoordinateSystem coordinate_system = ShapeCoordinateSystem::Model;
  POINT3 origin = kSphereOrigin;
  float_t parsed_radius = *radius.Get();
  if (IsUniformPositiveScaleAndTranslateOnly(model_to_world)) {
    POINT3 on_sphere =
        PointCreate(origin.x + parsed_radius, origin.y, origin.z);
    origin = PointMatrixMultiply(model_to_world.get(), origin);
    on_sphere = PointMatrixMultiply(model_to_world.get(), on_sphere);
    parsed_radius = on_sphere.x - origin.x;
    coordinate_system = ShapeCoordinateSystem::World;
  }

  Shape shape;
  ISTATUS status = EmissiveSphereAllocate(
      origin, parsed_radius, material.first.get(), material.first.get(),
      front_emissive_material.get(), back_emissive_material.get(),
      shape.release_and_get_address());
  SuccessOrOOM(status);

  std::vector<std::tuple<Shape, EmissiveMaterial, uint32_t>> emissive_faces;
  if (front_emissive_material.get()) {
    emissive_faces.push_back(
        std::make_tuple(shape, front_emissive_material, SPHERE_FRONT_FACE));
  }

  if (back_emissive_material.get()) {
    emissive_faces.push_back(
        std::make_tuple(shape, front_emissive_material, SPHERE_BACK_FACE));
  }

  std::vector<Shape> shapes;
  shapes.push_back(std::move(shape));

  return std::make_tuple(std::move(shapes), std::move(emissive_faces),
                         coordinate_system);
}

}  // namespace iris
#include "src/shapes/sphere.h"

#include <iostream>

#include "iris_physx_toolkit/sphere.h"
#include "src/common/error.h"
#include "src/common/ostream.h"
#include "src/param_matchers/float_single.h"
#include "src/param_matchers/matcher.h"

namespace iris {
namespace {

static const POINT3 kSphereOrigin = {(float_t)0.0, (float_t)0.0, (float_t)0.0};
static const float_t kSphereDefaultRadius = (float_t)1.0;

}  // namespace

ShapeResult ParseSphere(const char* base_type_name, const char* type_name,
                        Tokenizer& tokenizer, MaterialManager& material_manager,
                        const NamedTextureManager& named_texture_manager,
                        TextureManager& texture_manager,
                        SpectrumManager& spectrum_manager,
                        const MaterialFactory& material_factory,
                        const EmissiveMaterial& front_emissive_material,
                        const EmissiveMaterial& back_emissive_material) {
  SingleFloatMatcher radius(
      base_type_name, type_name, "radius", false, false, (float_t)0.0,
      std::numeric_limits<float_t>::infinity(), kSphereDefaultRadius);

  std::vector<Parameter> unused_parameters;
  MatchParameters<1>(base_type_name, type_name, tokenizer, {&radius},
                     &unused_parameters);

  auto material = material_factory.Build(
      base_type_name, type_name, unused_parameters, material_manager,
      named_texture_manager, texture_manager, spectrum_manager);

  Shape shape;
  ISTATUS status = EmissiveSphereAllocate(
      kSphereOrigin, *radius.Get(), material.get(), material.get(),
      front_emissive_material.get(), back_emissive_material.get(),
      shape.release_and_get_address());
  SuccessOrOOM(status);

  std::vector<std::tuple<Shape, EmissiveMaterial, uint32_t>> emissive_faces;
  if (front_emissive_material.get()) {
    emissive_faces.push_back(std::make_tuple(shape, front_emissive_material,
                                             SPHERE_FRONT_FACE));
  }

  if (back_emissive_material.get()) {
    emissive_faces.push_back(std::make_tuple(shape, front_emissive_material,
                                             SPHERE_BACK_FACE));
  }

  std::vector<Shape> shapes;
  shapes.push_back(std::move(shape));

  return std::make_pair(std::move(shapes), std::move(emissive_faces));
}

}  // namespace iris
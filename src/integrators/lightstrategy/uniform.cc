#include "src/integrators/lightstrategy/uniform.h"

#include <iostream>

#include "iris_physx_toolkit/one_light_sampler.h"
#include "src/common/error.h"

namespace iris {
namespace {

LightSampler CreateUniformLightSampler(std::vector<Light>& lights) {
  std::vector<PLIGHT> raw_lights;
  for (auto& light : lights) {
    raw_lights.push_back(light.get());
  }

  LightSampler result;
  ISTATUS status = OneLightSamplerAllocate(raw_lights.data(), raw_lights.size(),
                                           result.release_and_get_address());
  SuccessOrOOM(status);

  return result;
}

}  // namespace

LightSamplerFactory CreateUniformLightStrategy() {
  return CreateUniformLightSampler;
}

}  // namespace iris
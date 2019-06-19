#include "src/integrators/lightstrategy/parser.h"

#include <iostream>

#include "src/integrators/lightstrategy/uniform.h"

namespace iris {

LightSamplerFactory ParseLightStrategy(absl::string_view strategy) {
  if (strategy == "uniform") {
    return CreateUniformLightStrategy();
  }

  std::cerr << "ERROR: Unrecognized lightstrategy: " << strategy << std::endl;
  exit(EXIT_FAILURE);
}

}  // namespace iris
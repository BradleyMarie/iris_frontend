#include "src/directives/parse.h"

#include "src/directives/geometry.h"
#include "src/directives/global.h"

namespace iris {

RenderConfiguration ParseDirectives(Tokenizer& tokenizer, bool spectral,
                                    bool spectrum_color_workaround) {
  MatrixManager matrix_manager;
  auto global_config = ParseGlobalDirectives(tokenizer, matrix_manager,
                                             spectrum_color_workaround);
  SpectrumManager spectrum_manager(std::get<6>(global_config),
                                   std::get<7>(global_config), spectral);
  auto geometry_config = ParseGeometryDirectives(
      tokenizer, matrix_manager, spectrum_manager, std::get<6>(global_config),
      std::get<7>(global_config));
  return {
      std::move(geometry_config.first),
      std::move(std::get<5>(global_config)(geometry_config.second)),
      std::move(std::get<0>(global_config)),
      std::move(std::get<1>(global_config)),
      std::move(std::get<2>(global_config)),
      std::move(std::get<4>(global_config)),
      std::move(spectrum_manager.GetColorIntegrator()),
      std::move(std::get<9>(global_config)),
      std::move(std::get<3>(global_config)),
      std::move(std::get<8>(global_config)),
  };
}

}  // namespace iris
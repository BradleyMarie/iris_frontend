#ifndef _SRC_DIRECTIVES_PARSER_
#define _SRC_DIRECTIVES_PARSER_

#include <tuple>

#include "src/common/pointer_types.h"
#include "src/common/tokenizer.h"
#include "src/films/output_writers/result.h"

namespace iris {

typedef std::tuple<Scene, LightSampler, Camera, Matrix, Sampler, Integrator,
                   ColorIntegrator, Random, Framebuffer, OutputWriter>
    RendererConfiguration;

struct SpectralRepresentation {
  absl::optional<COLOR_SPACE> color_space;
};

class Parser {
 public:
  Parser() = default;
  Parser(const Parser&) = delete;
  Parser& operator=(const Parser&) = delete;
  Parser(Parser&& other) = default;
  Parser& operator=(Parser&& other) = default;

  static Parser Create(absl::string_view file_path);
  static Parser Create(std::istream& stream);

  absl::optional<RendererConfiguration> Next(
    absl::optional<SpectralRepresentation> spectral_representation_override,
    absl::optional<COLOR_SPACE> rgb_color_space_override,
    absl::optional<bool> always_compute_reflective_color_override);
  bool Done();

 private:
  Tokenizer m_tokenizer;
};

}  // namespace iris

#endif  // _SRC_DIRECTIVES_PARSER_
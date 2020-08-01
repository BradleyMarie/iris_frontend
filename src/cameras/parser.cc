#include "src/cameras/parser.h"

#include "src/cameras/perspective.h"

namespace iris {
namespace {

const Directive::Implementations<CameraFactory> kImpls = {
    {"perspective", ParsePerspective}};

}  // namespace

CameraFactory ParseCamera(Directive& directive) {
  return directive.Invoke(kImpls);
}

CameraFactory CreateDefaultCamera() {
  Parameters parameters;
  return ParsePerspective(parameters);
}

}  // namespace iris
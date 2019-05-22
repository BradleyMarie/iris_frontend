#include <array>
#include <iostream>

#include "absl/strings/numbers.h"
#include "src/matrix_parser.h"
#include "src/quoted_string.h"

namespace iris {
namespace {

template <size_t N>
void ParseFloats(Tokenizer& tokenizer, const char* name,
                 std::array<float_t, N>& array) {
  for (size_t i = 0; i < N; i++) {
    auto token = tokenizer.Next();
    if (!token) {
      std::cerr << "ERROR: " << name << " requires " << N << " parameters"
                << std::endl;
      exit(EXIT_FAILURE);
    }

    if (!absl::SimpleAtof(*token, &array[i])) {
      std::cerr << "ERROR: Failed to parse " << name << " parameter: " << *token
                << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

static void ParseTranslate(Tokenizer& tokenizer,
                           MatrixManager& matrix_manager) {
  std::array<float_t, 3> params;
  ParseFloats(tokenizer, "Translate", params);
  matrix_manager.Translate(params[0], params[1], params[2]);
}

static void ParseScale(Tokenizer& tokenizer, MatrixManager& matrix_manager) {
  std::array<float_t, 3> params;
  ParseFloats(tokenizer, "Scale", params);
  matrix_manager.Scale(params[0], params[1], params[2]);
}

static void ParseRotate(Tokenizer& tokenizer, MatrixManager& matrix_manager) {
  std::array<float_t, 4> params;
  ParseFloats(tokenizer, "Rotate", params);
  matrix_manager.Rotate(params[0], params[1], params[2], params[3]);
}

static void ParseLookAt(Tokenizer& tokenizer, MatrixManager& matrix_manager) {
  std::array<float_t, 9> params;
  ParseFloats(tokenizer, "LookAt", params);
  matrix_manager.LookAt(params[0], params[1], params[2], params[3], params[4],
                        params[5], params[6], params[7], params[8]);
}

static void ParseTransform(Tokenizer& tokenizer,
                           MatrixManager& matrix_manager) {
  std::array<float_t, 16> params;
  ParseFloats(tokenizer, "Transform", params);
  matrix_manager.Transform(params[0], params[1], params[2], params[3],
                           params[4], params[5], params[6], params[7],
                           params[8], params[9], params[10], params[11],
                           params[12], params[13], params[14], params[15]);
}

static void ParseConcatTransform(Tokenizer& tokenizer,
                                 MatrixManager& matrix_manager) {
  std::array<float_t, 16> params;
  ParseFloats(tokenizer, "ConcatTransform", params);
  matrix_manager.Transform(params[0], params[1], params[2], params[3],
                           params[4], params[5], params[6], params[7],
                           params[8], params[9], params[10], params[11],
                           params[12], params[13], params[14], params[15]);
}

static void ParseCoordinateSystem(Tokenizer& tokenizer,
                                  MatrixManager& matrix_manager) {
  auto token = tokenizer.Next();
  if (!token) {
    std::cerr << "ERROR: CoordinateSystem requires 1 parameter" << std::endl;
    exit(EXIT_FAILURE);
  }

  auto unquoted = UnquoteToken(*token);
  if (!unquoted) {
    std::cerr << "ERROR: Failed to parse CoordinateSystem parameter: " << *token
              << std::endl;
    exit(EXIT_FAILURE);
  }

  matrix_manager.CoordinateSystem(*unquoted);
}

static void ParseCoordSysTransform(Tokenizer& tokenizer,
                                   MatrixManager& matrix_manager) {
  auto token = tokenizer.Next();
  if (!token) {
    std::cerr << "ERROR: CoordSysTransform requires 1 parameter" << std::endl;
    exit(EXIT_FAILURE);
  }

  auto unquoted = UnquoteToken(*token);
  if (!unquoted) {
    std::cerr << "ERROR: Failed to parse CoordSysTransform parameter: "
              << *token << std::endl;
    exit(EXIT_FAILURE);
  }

  matrix_manager.CoordSysTransform(*unquoted);
}

static void ParseActiveTransform(Tokenizer& tokenizer,
                                 MatrixManager& matrix_manager) {
  auto token = tokenizer.Next();
  if (!token) {
    std::cerr << "ERROR: ActiveTransform requires 1 parameter" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (*token == "StartTime") {
    matrix_manager.ActiveTransform(MatrixManager::START_TRANSFORM);
    return;
  }

  if (*token == "EndTime") {
    matrix_manager.ActiveTransform(MatrixManager::END_TRANSFORM);
    return;
  }

  if (*token == "All") {
    matrix_manager.ActiveTransform(MatrixManager::ALL_TRANSFORMS);
    return;
  }

  std::cerr << "ERROR: Failed to parse ActiveTransform parameter: " << *token
            << std::endl;
  exit(EXIT_FAILURE);
}

}  // namespace

bool TryParseMatrix(absl::string_view directive, Tokenizer& tokenizer,
                    MatrixManager& matrix_manager) {
  if (directive == "Identity") {
    matrix_manager.Identity();
    return true;
  }

  if (directive == "Translate") {
    ParseTranslate(tokenizer, matrix_manager);
    return true;
  }

  if (directive == "Scale") {
    ParseScale(tokenizer, matrix_manager);
    return true;
  }

  if (directive == "Rotate") {
    ParseRotate(tokenizer, matrix_manager);
    return true;
  }

  if (directive == "LookAt") {
    ParseLookAt(tokenizer, matrix_manager);
    return true;
  }

  if (directive == "CoordinateSystem") {
    ParseCoordinateSystem(tokenizer, matrix_manager);
    return true;
  }

  if (directive == "CoordSysTransform") {
    ParseCoordSysTransform(tokenizer, matrix_manager);
    return true;
  }

  if (directive == "Transform") {
    ParseTransform(tokenizer, matrix_manager);
    return true;
  }

  if (directive == "ConcatTransform") {
    ParseConcatTransform(tokenizer, matrix_manager);
    return true;
  }

  if (directive == "ActiveTransform") {
    ParseActiveTransform(tokenizer, matrix_manager);
    return true;
  }

  return false;
}

}  // namespace iris
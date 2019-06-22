#include <array>
#include <iostream>

#include "absl/strings/numbers.h"
#include "src/common/quoted_string.h"
#include "src/matrix_parser.h"

namespace iris {
namespace {

template <size_t N>
void ParseFiniteFloats(Tokenizer& tokenizer, const char* name,
                       std::array<FiniteFloatT, N>& array,
                       std::array<std::string, N>* strings = nullptr) {
  for (size_t i = 0; i < N; i++) {
    auto token = tokenizer.Next();
    if (!token) {
      std::cerr << "ERROR: " << name << " requires " << N << " parameters"
                << std::endl;
      exit(EXIT_FAILURE);
    }

    if (!ParseFiniteFloatT(*token, &array[i])) {
      std::cerr << "ERROR: Failed to parse " << name
                << " parameter to a finite floating point value: " << *token
                << std::endl;
      exit(EXIT_FAILURE);
    }

    if (strings) {
      (*strings)[i].assign(token->begin(), token->size());
    }
  }
}

template <size_t N>
void ParseFiniteNonZeroFloats(Tokenizer& tokenizer, const char* name,
                              std::array<FiniteNonZeroFloatT, N>& array) {
  for (size_t i = 0; i < N; i++) {
    auto token = tokenizer.Next();
    if (!token) {
      std::cerr << "ERROR: " << name << " requires " << N << " parameters"
                << std::endl;
      exit(EXIT_FAILURE);
    }

    if (!ParseFiniteNonZeroFloatT(*token, &array[i])) {
      std::cerr << "ERROR: Failed to parse " << name
                << " parameter to a finite, non-zero floating point value: "
                << *token << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

static void ParseTranslate(Tokenizer& tokenizer,
                           MatrixManager& matrix_manager) {
  std::array<FiniteFloatT, 3> params;
  ParseFiniteFloats(tokenizer, "Translate", params);
  matrix_manager.Translate(params);
}

static void ParseScale(Tokenizer& tokenizer, MatrixManager& matrix_manager) {
  std::array<FiniteNonZeroFloatT, 3> params;
  ParseFiniteNonZeroFloats(tokenizer, "Scale", params);
  matrix_manager.Scale(params);
}

static void ParseRotate(Tokenizer& tokenizer, MatrixManager& matrix_manager) {
  std::array<FiniteFloatT, 4> params;
  ParseFiniteFloats(tokenizer, "Rotate", params);
  matrix_manager.Rotate(params);
}

static void ParseLookAt(Tokenizer& tokenizer, MatrixManager& matrix_manager) {
  std::array<FiniteFloatT, 9> params;
  ParseFiniteFloats(tokenizer, "LookAt", params);
  matrix_manager.LookAt(params);
}

static void ParseTransform(Tokenizer& tokenizer,
                           MatrixManager& matrix_manager) {
  std::array<FiniteFloatT, 16> params;
  std::array<std::string, 16> strings;
  ParseFiniteFloats(tokenizer, "Transform", params, &strings);
  matrix_manager.Transform(params, strings);
}

static void ParseConcatTransform(Tokenizer& tokenizer,
                                 MatrixManager& matrix_manager) {
  std::array<FiniteFloatT, 16> params;
  std::array<std::string, 16> strings;
  ParseFiniteFloats(tokenizer, "ConcatTransform", params, &strings);
  matrix_manager.ConcatTransform(params, strings);
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
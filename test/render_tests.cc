#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>

#include "googletest/include/gtest/gtest.h"
#include "src/render.h"

using iris::Parser;

namespace {

bool IsLittleEndian() {
  union {
    uint16_t value;
    char bytes[2];
  } value = {0x0001};

  return value.bytes[0] == 1;
}

void SwapBytes(void* bytes, size_t size) {
  char* end = (char*)bytes + size - 1;
  char* begin = (char*)bytes;

  while (begin < end) {
    char temp = *begin;
    *begin = *end;
    *end = temp;

    begin++;
    end--;
  }
}

void ValidateMagicNumber(FILE* file) {
  ASSERT_EQ('P', fgetc(file));
  ASSERT_EQ('F', fgetc(file));
  ASSERT_EQ('\n', fgetc(file));
}

void GetSize(FILE* file, size_t* xres, size_t* yres) {
  ASSERT_EQ(2, fscanf(file, "%zu %zu", xres, yres));
}

void ByteSwapNeeded(FILE* file, bool* swap_needed) {
  float endianness;
  ASSERT_EQ(1, fscanf(file, "%f", &endianness));
  *swap_needed = (endianness < 0.0f) != IsLittleEndian();
}

void CheckEquals(const char* expected, const iris::Framebuffer& actual,
                 float epsilon) {
  FILE* left = fopen(expected, "rb");
  ASSERT_NE(left, nullptr);
  ValidateMagicNumber(left);

  size_t expected_xres, expected_yres;
  GetSize(left, &expected_xres, &expected_yres);

  size_t actual_xres, actual_yres;
  FramebufferGetSize(actual.get(), &actual_xres, &actual_yres);
  ASSERT_EQ(expected_xres, actual_xres);
  ASSERT_EQ(expected_yres, actual_yres);

  bool swap_needed = false;
  ByteSwapNeeded(left, &swap_needed);

  ASSERT_EQ('\n', fgetc(left));

  for (size_t y = 0; y < actual_yres; y++) {
    for (size_t x = 0; x < actual_xres; x++) {
      float subpixels[3];
      ASSERT_EQ(3u, fread(subpixels, sizeof(float), 3, left));

      if (swap_needed) {
        SwapBytes(&subpixels[0], sizeof(float));
        SwapBytes(&subpixels[1], sizeof(float));
        SwapBytes(&subpixels[2], sizeof(float));
      }

      float_t values[3] = {(float_t)subpixels[0], (float_t)subpixels[1],
                           (float_t)subpixels[2]};
      COLOR3 expected_color = ColorCreate(COLOR_SPACE_LINEAR_SRGB, values);

      COLOR3 actual_color;
      FramebufferGetPixel(actual.get(), x, actual_yres - 1 - y, &actual_color);
      actual_color = ColorConvert(actual_color, expected_color.color_space);

      EXPECT_NEAR(expected_color.values[0], actual_color.values[0], epsilon);
      EXPECT_NEAR(expected_color.values[1], actual_color.values[1], epsilon);
      EXPECT_NEAR(expected_color.values[2], actual_color.values[2], epsilon);
    }
  }

  fclose(left);
}

std::pair<Parser, std::unique_ptr<std::stringstream>> CreateParserFromString(
    const std::string& string_to_parse) {
  auto buffer = absl::make_unique<std::stringstream>(string_to_parse);
  auto tokenizer = Parser::Create(*buffer);
  return std::make_pair(std::move(tokenizer), std::move(buffer));
}

static const size_t kRenderIndex = 0;
static const float_t kEpsilon = (float_t)0.001;
static const size_t kNumThreads = 1;
static const bool kReportProgress = false;
static const absl::optional<iris::SpectralRepresentation>
    kOverrideSpectralRepresentation = absl::nullopt;
static const absl::optional<COLOR_SPACE> kRgbColorSpace = absl::nullopt;
static const absl::optional<bool> kSpectrumColorWorkaround = absl::nullopt;

}  // namespace

TEST(RenderTests, CornellBox) {
  auto parser = Parser::Create("test/cornell_box.pbrt");
  auto render_result =
      RenderToFramebuffer(parser, kRenderIndex, kEpsilon, kNumThreads,
                          kReportProgress, kOverrideSpectralRepresentation,
                          kRgbColorSpace, kSpectrumColorWorkaround);
  CheckEquals("test/cornell_box.pfm", render_result.first, (float_t)0.1);
}

TEST(RenderTests, IncludeCornellBox) {
  auto parser = CreateParserFromString("Include \"test/cornell_box.pbrt\"");
  auto render_result =
      RenderToFramebuffer(parser.first, kRenderIndex, kEpsilon, kNumThreads,
                          kReportProgress, kOverrideSpectralRepresentation,
                          kRgbColorSpace, kSpectrumColorWorkaround);
  CheckEquals("test/cornell_box.pfm", render_result.first, (float_t)0.1);
}
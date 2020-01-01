#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>

#include "googletest/include/gtest/gtest.h"
#include "src/render.h"

using iris::Tokenizer;

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

      COLOR3 pixel_color;
      FramebufferGetPixel(actual.get(), x, actual_yres - 1 - y, &pixel_color);
      float r = 3.2404542f * (float)pixel_color.x -
                1.5371385f * (float)pixel_color.y -
                0.4985314f * (float)pixel_color.z;

      float g = -0.969266f * (float)pixel_color.x +
                1.8760108f * (float)pixel_color.y +
                0.0415560f * (float)pixel_color.z;

      float b = 0.0556434f * (float)pixel_color.x -
                0.2040259f * (float)pixel_color.y +
                1.0572252f * (float)pixel_color.z;

      r = fmaxf(0.0f, r);
      g = fmaxf(0.0f, g);
      b = fmaxf(0.0f, b);

      EXPECT_NEAR(subpixels[0], r, epsilon);
      EXPECT_NEAR(subpixels[1], g, epsilon);
      EXPECT_NEAR(subpixels[2], b, epsilon);
    }
  }

  fclose(left);
}

std::pair<std::unique_ptr<Tokenizer>, std::unique_ptr<std::stringstream>>
CreateTokenizerFromString(const std::string& string_to_parse) {
  auto buffer = absl::make_unique<std::stringstream>(string_to_parse);
  auto tokenizer = Tokenizer::CreateFromStream(*buffer);
  return std::make_pair(std::move(tokenizer), std::move(buffer));
}

static const float_t kEpsilon = (float_t)0.001;
static const std::string kSearchDir(".");
static const size_t kNumThreads = 1;

}  // namespace

TEST(RenderTests, CornellBox) {
  auto tokenizer = Tokenizer::CreateFromFile("test/cornell_box.pbrt");
  auto render_result =
      RenderToFramebuffer(*tokenizer, kSearchDir, kEpsilon, kNumThreads);
  CheckEquals("test/cornell_box.pfm", render_result.first, (float_t)0.1);
}

TEST(RenderTests, IncludeCornellBox) {
  auto tokenizer =
      CreateTokenizerFromString("Include \"test/cornell_box.pbrt\"");
  auto render_result =
      RenderToFramebuffer(*tokenizer.first, kSearchDir, kEpsilon, kNumThreads);
  CheckEquals("test/cornell_box.pfm", render_result.first, (float_t)0.1);
}
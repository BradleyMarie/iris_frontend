#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>

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
  if (fgetc(file) != 'P' || fgetc(file) != 'F' || fgetc(file) != '\n') {
    std::cerr << "ERROR: Bad magic number" << std::endl;
    exit(EXIT_FAILURE);
  }
}

std::pair<size_t, size_t> GetSize(FILE* file) {
  size_t xres, yres;
  if (fscanf(file, "%zu %zu", &xres, &yres) != 2) {
    std::cerr << "ERROR: Failed to parse size" << std::endl;
    exit(EXIT_FAILURE);
  }

  return std::make_pair(xres, yres);
}

bool ByteSwapNeeded(FILE* file) {
  float endianness;
  if (fscanf(file, "%f", &endianness) != 1) {
    std::cerr << "ERROR: Failed to parse endianness" << std::endl;
    exit(EXIT_FAILURE);
  }

  return (endianness < 0.0f) == IsLittleEndian();
}

void CheckEquals(float epsilon, FILE* left, FILE* right) {
  ValidateMagicNumber(left);
  ValidateMagicNumber(right);

  if (GetSize(left) != GetSize(right)) {
    std::cerr << "ERROR: Size mismatch" << std::endl;
    exit(EXIT_FAILURE);
  }

  bool swap_left = ByteSwapNeeded(left);
  bool swap_right = ByteSwapNeeded(right);

  while (true) {
    float left_value;
    size_t left_read = fread(&left_value, sizeof(float), 1, left);

    float right_value;
    size_t right_read = fread(&right_value, sizeof(float), 1, right);

    if (left_read != right_read) {
      std::cerr << "ERROR: File length mismatch" << std::endl;
      exit(EXIT_FAILURE);
    }

    if (left_read == 0) {
      return;
    }

    if (swap_left) {
      SwapBytes(&left_value, sizeof(float));
    }

    if (swap_right) {
      SwapBytes(&right_value, sizeof(float));
    }

    if (epsilon < std::abs(left_value - right_value)) {
      std::cerr << "ERROR: Pixel value mismatch" << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

}  // namespace

int main(int argc, char** argv) {
  if (argc != 4 || (std::strcmp(argv[1], "exact") != 0 &&
                    std::strcmp(argv[1], "approximate") != 0)) {
    std::cout << "usage: pfm_matches <exact|approximate> <left> <right>"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  FILE* left = fopen(argv[2], "rb");
  if (left == NULL) {
    std::cout << "ERROR: Failed to open left file" << std::endl;
    exit(EXIT_FAILURE);
  }

  FILE* right = fopen(argv[3], "rb");
  if (right == NULL) {
    std::cout << "ERROR: Failed to open right file" << std::endl;
    exit(EXIT_FAILURE);
  }

  float epsilon;
  if (std::strcmp(argv[1], "exact") == 0) {
    epsilon = 0.000f;
  } else {
    epsilon = 0.001f;
  }

  CheckEquals(epsilon, left, right);
  fclose(left);
  fclose(right);

  return EXIT_SUCCESS;
}
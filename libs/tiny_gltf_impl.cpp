// 1. Define Implementation for TinyGLTF
#define TINYGLTF_IMPLEMENTATION

// 2. Disable internal STB inclusion (because we link it from STBImpl.cpp)
#define TINYGLTF_NO_INCLUDE_STB_IMAGE
#define TINYGLTF_NO_INCLUDE_STB_IMAGE_WRITE

// 3. CRITICAL: Include the STB headers so TinyGLTF sees the function definitions
#include "stb_image.h"
#include "stb_image_write.h"

#include "json.hpp"

// 4. Finally, include TinyGLTF
#include "tiny_gltf.h"
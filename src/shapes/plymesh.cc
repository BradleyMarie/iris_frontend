#include "src/shapes/plymesh.h"

#include <cstdio>
#include <cstring>
#include <iostream>

#include "iris_physx_toolkit/triangle_mesh.h"
#include "src/common/error.h"
#include "src/common/ostream.h"
#include "src/param_matchers/matcher.h"
#include "src/param_matchers/single.h"
#include "third_party/rply/rply.h"
#include "third_party/rply/rplyfile.h"

namespace iris {
namespace {

static const std::string kPlyMeshDefaultFilename;

static const int kRplySuccess = 1;

static const long kFlagX = 0;
static const long kFlagY = 1;
static const long kFlagZ = 2;
static const long kFlagU = 0;
static const long kFlagV = 1;

class PlyData {
 public:
  PlyData(size_t num_vertices) { m_vertices.resize(num_vertices); }

  void SetVertexX(size_t index, float_t value) {
    assert(index < m_vertices.size());

    if (!isfinite(value)) {
      std::cerr << "ERROR: PLY file contained a 'vertex' element with a "
                   "non-finite value for 'x' at index: "
                << index << std::endl;
      exit(EXIT_FAILURE);
    }

    m_vertices[index].x = value;
  }

  void SetVertexY(size_t index, float_t value) {
    assert(index < m_vertices.size());

    if (!isfinite(value)) {
      std::cerr << "ERROR: PLY file contained a 'vertex' element with a "
                   "non-finite value for 'y' at index: "
                << index << std::endl;
      exit(EXIT_FAILURE);
    }

    m_vertices[index].y = value;
  }

  void SetVertexZ(size_t index, float_t value) {
    assert(index < m_vertices.size());

    if (!isfinite(value)) {
      std::cerr << "ERROR: PLY file contained a 'vertex' element with a "
                   "non-finite value for 'z' at index: "
                << index << std::endl;
      exit(EXIT_FAILURE);
    }

    m_vertices[index].y = value;
  }

  void AllocateNormals() { m_normals.resize(m_vertices.size()); }

  void SetNormalX(size_t index, float_t value) {
    assert(index < m_normals.size());

    if (!isfinite(value)) {
      std::cerr << "ERROR: PLY file contained a 'vertex' element with a "
                   "non-finite value for 'nx' at index: "
                << index << std::endl;
      exit(EXIT_FAILURE);
    }

    m_normals[index].x = value;
  }

  void SetNormalY(size_t index, float_t value) {
    assert(index < m_normals.size());

    if (!isfinite(value)) {
      std::cerr << "ERROR: PLY file contained a 'vertex' element with a "
                   "non-finite value for 'ny' at index: "
                << index << std::endl;
      exit(EXIT_FAILURE);
    }

    m_normals[index].y = value;
  }

  void SetNormalZ(size_t index, float_t value) {
    assert(index < m_normals.size());

    if (!isfinite(value)) {
      std::cerr << "ERROR: PLY file contained a 'vertex' element with a "
                   "non-finite value for 'nz' at index: "
                << index << std::endl;
      exit(EXIT_FAILURE);
    }

    m_normals[index].z = value;
  }

  void AllocateUv() { m_uvs.resize(m_vertices.size()); }

  void SetU(size_t index, float_t value) {
    assert(index < m_uvs.size());

    if (!isfinite(value)) {
      std::cerr << "ERROR: PLY file contained a 'vertex' element with a "
                   "non-finite value for '"
                << m_u_name << "' at index: " << index << std::endl;
      exit(EXIT_FAILURE);
    }

    m_uvs[index].first = value;
  }

  void SetV(size_t index, float_t value) {
    assert(index < m_uvs.size());

    if (!isfinite(value)) {
      std::cerr << "ERROR: PLY file contained a 'vertex' element with a "
                   "non-finite value for '"
                << m_v_name << "' at index: " << index << std::endl;
      exit(EXIT_FAILURE);
    }

    m_uvs[index].second = value;
  }

  void AddTriangleFaceIndex(size_t index) {
    if (m_vertices.size() <= index) {
      std::cerr << "ERROR: PLY file contained a 'face' element with an out of "
                   "bounds value for 'vertex_indices': "
                << index << std::endl;
      exit(EXIT_FAILURE);
    }

    m_faces.push_back(index);
  }

  void AddQuadFaceIndex(size_t index) {
    assert(!m_faces.empty() && m_faces.size() % 3 == 0);

    if (m_vertices.size() <= index) {
      std::cerr << "ERROR: PLY file contained a 'face' element with an out of "
                   "bounds value for 'vertex_indices': "
                << index << std::endl;
      exit(EXIT_FAILURE);
    }

    size_t triangle_begin = m_faces.size() - 3;
    m_faces.push_back(m_faces[triangle_begin]);
    m_faces.push_back(m_faces[triangle_begin + 2]);
    m_faces.push_back(index);
  }

  void SetUName(const std::string& u_name) { m_u_name = u_name; }

  void SetVName(const std::string& v_name) { m_v_name = v_name; }

  const std::vector<POINT3>& GetVertices() const { return m_vertices; }

  const std::vector<VECTOR3>& GetNormals() const { return m_normals; }

  const std::vector<std::pair<float_t, float_t>>& GetUVs() const {
    return m_uvs;
  }

  const std::vector<size_t>& GetFaces() const { return m_faces; }

 private:
  std::vector<POINT3> m_vertices;
  std::vector<VECTOR3> m_normals;
  std::vector<std::pair<float_t, float_t>> m_uvs;
  std::vector<size_t> m_faces;
  std::string m_filename;
  std::string m_u_name;
  std::string m_v_name;
};

static bool FitsSizeT(long value) {
  if (value < 0) {
    return false;
  }

#if SIZE_MAX < LONG_MAX
  if ((long)SIZE_MAX < value) {
    return false;
  }
#endif

  return true;
}

static bool AsSizeT(long value, size_t* output) {
  if (!FitsSizeT(value)) {
    return false;
  }

  *output = (size_t)value;

  return true;
}

static int PlyVertexCallback(p_ply_argument argument) {
  void* context;
  long flags;
  ply_get_argument_user_data(argument, &context, &flags);

  PlyData* data = static_cast<PlyData*>(context);

  long index;
  ply_get_argument_element(argument, NULL, &index);
  assert(FitsSizeT(index));

  float_t value = ply_get_argument_value(argument);

  switch (flags) {
    case kFlagX:
      data->SetVertexX((long)index, value);
      break;
    case kFlagY:
      data->SetVertexY((long)index, value);
      break;
    case kFlagZ:
      data->SetVertexZ((long)index, value);
      break;
    default:
      assert(false);
  }

  return kRplySuccess;
}

static int PlyNormalCallback(p_ply_argument argument) {
  void* context;
  long flags;
  ply_get_argument_user_data(argument, &context, &flags);

  PlyData* data = static_cast<PlyData*>(context);

  long index;
  ply_get_argument_element(argument, NULL, &index);
  assert(FitsSizeT(index));

  float_t value = ply_get_argument_value(argument);

  switch (flags) {
    case kFlagX:
      data->SetNormalX((long)index, value);
      break;
    case kFlagY:
      data->SetNormalY((long)index, value);
      break;
    case kFlagZ:
      data->SetNormalZ((long)index, value);
      break;
    default:
      assert(false);
  }

  return kRplySuccess;
}

static int PlyUvCallback(p_ply_argument argument) {
  void* context;
  long flags;
  ply_get_argument_user_data(argument, &context, &flags);

  PlyData* data = static_cast<PlyData*>(context);

  long index;
  ply_get_argument_element(argument, NULL, &index);
  assert(FitsSizeT(index));

  float_t value = ply_get_argument_value(argument);

  switch (flags) {
    case kFlagU:
      data->SetU((long)index, value);
      break;
    case kFlagV:
      data->SetV((long)index, value);
      break;
    default:
      assert(false);
  }

  return kRplySuccess;
}

static int PlyVertexIndiciesCallback(p_ply_argument argument) {
  void* context;
  long flags;
  ply_get_argument_user_data(argument, &context, &flags);

  PlyData* data = static_cast<PlyData*>(context);

  long length, index;
  ply_get_argument_property(argument, NULL, &length, &index);

  assert(0 <= index && index <= 3);
  if (index == -1) {
    return kRplySuccess;
  }

  if (length != 3 || 4 != length) {
    std::cerr << "ERROR: Only triangles and quads are currently supported by "
                 "plymesh Shapes, but PLY file contained a 'face' element "
                 "which contained a 'vertex_indices' of an unsupported length: "
              << length << std::endl;
    exit(EXIT_FAILURE);
  }

  long value = (long)ply_get_argument_value(argument);

  size_t vertex_index;
  if (!AsSizeT(value, &vertex_index)) {
    std::cerr << "ERROR: PLY file contained a 'face' element with an out of "
                 "bounds value in its 'vertex_indices': "
              << value << std::endl;
    exit(EXIT_FAILURE);
  }

  if (index <= 2) {
    data->AddTriangleFaceIndex(vertex_index);
  } else {
    data->AddQuadFaceIndex(vertex_index);
  }

  return kRplySuccess;
}

static void InitializeVertexCallbacks(p_ply ply, PlyData* ply_data,
                                      size_t num_vertices) {
  assert(ply != NULL);
  assert(ply_data != NULL);

  long num_x =
      ply_set_read_cb(ply, "vertex", "x", PlyVertexCallback, ply_data, kFlagX);

  if (num_x != (long)num_vertices) {
    std::cerr << "ERROR: PLY file must contain exactly one 'x' value for each "
                 "'vertex' element"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  long num_y =
      ply_set_read_cb(ply, "vertex", "y", PlyVertexCallback, ply_data, kFlagY);

  if (num_y != (long)num_vertices) {
    std::cerr << "ERROR: PLY file must contain exactly one 'y' value for each "
                 "'vertex' element"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  long num_z =
      ply_set_read_cb(ply, "vertex", "z", PlyVertexCallback, ply_data, kFlagZ);

  if (num_z != (long)num_vertices) {
    std::cerr << "ERROR: PLY file must contain exactly one 'z' value for each "
                 "'vertex' element"
              << std::endl;
    exit(EXIT_FAILURE);
  }
}

static void InitializeNormalCallbacks(p_ply ply, PlyData* ply_data,
                                      size_t num_vertices) {
  assert(ply != NULL);
  assert(ply_data != NULL);

  long num_nx =
      ply_set_read_cb(ply, "vertex", "nx", PlyNormalCallback, ply_data, kFlagX);

  long expected_normals;
  if (num_nx != 0) {
    if (num_nx != (long)num_vertices) {
      std::cerr
          << "ERROR: PLY file must contain exactly one 'nx' value for each "
             "'vertex' element if normals are specified"
          << std::endl;
      exit(EXIT_FAILURE);
    }

    ply_data->AllocateNormals();
    expected_normals = num_vertices;
  } else {
    expected_normals = 0;
  }

  long num_ny =
      ply_set_read_cb(ply, "vertex", "ny", PlyNormalCallback, ply_data, kFlagY);

  if (num_ny != expected_normals) {
    std::cerr << "ERROR: PLY file must contain exactly one 'ny' value for each "
                 "'vertex' element if normals are specified"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  long num_nz =
      ply_set_read_cb(ply, "vertex", "nz", PlyNormalCallback, ply_data, kFlagZ);

  if (num_nz != expected_normals) {
    std::cerr << "ERROR: PLY file must contain exactly one 'nz' value for each "
                 "'vertex' element if normals are specified"
              << std::endl;
    exit(EXIT_FAILURE);
  }
}

static void InitializeUVCallback(p_ply ply, PlyData* ply_data,
                                 const std::string& u_name,
                                 const std::string& v_name, size_t num_vertices,
                                 bool& found) {
  assert(ply != NULL);
  assert(ply_data != NULL);

  long num_us = ply_set_read_cb(ply, "vertex", u_name.c_str(), PlyUvCallback,
                                ply_data, kFlagU);

  long expected_vs;
  if (found) {
    if (num_us != 0) {
      std::cerr << "ERROR: PLY file can only use a single kind of UV "
                   "properties for its 'vertex' elements"
                << std::endl;
      exit(EXIT_FAILURE);
    }

    expected_vs = 0;
  } else {
    if (num_us != 0) {
      if (num_us != (long)num_vertices) {
        std::cerr << "ERROR: PLY file must contain exactly one '" << u_name
                  << "' value for each 'vertex' element if '" << u_name
                  << "' or '" << v_name << "' is specified" << std::endl;
        exit(EXIT_FAILURE);
      }

      found = true;
      expected_vs = (long)num_vertices;
      ply_data->AllocateUv();
      ply_data->SetUName(u_name);
      ply_data->SetVName(v_name);
    } else {
      expected_vs = 0;
    }
  }

  long num_vs = ply_set_read_cb(ply, "vertex", v_name.c_str(), PlyUvCallback,
                                ply_data, kFlagV);

  if (num_vs != expected_vs) {
    if (expected_vs == 0) {
      std::cerr << "ERROR: PLY file can only use a single kind of UV "
                   "properties for its 'vertex' elements"
                << std::endl;
      exit(EXIT_FAILURE);
    }

    std::cerr << "ERROR: PLY file must contain exactly one '" << u_name
              << "' value for each 'vertex' element if '" << u_name << "' or '"
              << v_name << "' is specified" << std::endl;
    exit(EXIT_FAILURE);
  }
}

static void InitializeUVCallbacks(p_ply ply, PlyData* ply_data,
                                  size_t num_vertices) {
  assert(ply != NULL);
  assert(ply_data != NULL);

  bool found = false;
  InitializeUVCallback(ply, ply_data, "u", "v", num_vertices, found);
  InitializeUVCallback(ply, ply_data, "s", "t", num_vertices, found);
  InitializeUVCallback(ply, ply_data, "texture_u", "texture_v", num_vertices,
                       found);
  InitializeUVCallback(ply, ply_data, "texture_s", "texture_t", num_vertices,
                       found);
}

PlyData ReadPlyFile(const std::string& file_name) {
  FILE* file = fopen(file_name.c_str(), "rb");
  if (!file) {
    std::cerr << "ERROR: Failed to open PLY file: " << file_name << std::endl;
    exit(EXIT_FAILURE);
  }

  p_ply ply = ply_open_from_file(file, nullptr, 0, nullptr);
  if (!ply) {
    fclose(file);
    ReportOOM();
  }

  if (ply_read_header(ply) == 0) {
    std::cerr << "ERROR: Malformed PLY file: " << file_name << std::endl;
    ply_close(ply);
    fclose(file);
    exit(EXIT_FAILURE);
  }

  size_t num_faces = 0;
  size_t num_vertices = 0;

  for (p_ply_element element = ply_get_next_element(ply, NULL); element != NULL;
       element = ply_get_next_element(ply, element)) {
    const char* element_name;
    long num_instances;

    ply_get_element_info(element, &element_name, &num_instances);

    if (strcmp(element_name, "face") == 0) {
      if (!AsSizeT(num_instances, &num_faces)) {
        std::cerr << "ERROR: PLY file contained an unsupported number of "
                     "'face' elements: "
                  << num_instances << std::endl;
        ply_close(ply);
        fclose(file);
        exit(EXIT_FAILURE);
      }
    } else if (strcmp(element_name, "vertex") == 0) {
      if (!AsSizeT(num_instances, &num_vertices)) {
        std::cerr << "ERROR: PLY file contained an unsupported number of "
                     "'vertex' elements: "
                  << num_instances << std::endl;
        ply_close(ply);
        fclose(file);
        exit(EXIT_FAILURE);
      }
    }
  }

  PlyData context(num_vertices);
  InitializeVertexCallbacks(ply, &context, num_vertices);
  InitializeNormalCallbacks(ply, &context, num_vertices);
  InitializeUVCallbacks(ply, &context, num_vertices);

  long vertex_indices = ply_set_read_cb(ply, "face", "vertex_indices",
                                        PlyVertexIndiciesCallback, &context, 0);
  if (vertex_indices != (long)num_faces) {
    std::cerr << "ERROR: PLY file must contain exactly one 'vertex_indices' "
                 "tuple for each 'face' element"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  int read_status = ply_read(ply);
  ply_close(ply);
  fclose(file);

  if (read_status != kRplySuccess) {
    std::cerr << "ERROR: Unexpected failure reading PLY file" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (context.GetFaces().size() % 3 != 0) {
    std::cerr
        << "ERROR: PLY file generated a triangle with fewer than 3 vertices"
        << std::endl;
    exit(EXIT_FAILURE);
  }

  for (const auto& vertex : context.GetVertices()) {
    if (!PointValidate(vertex)) {
      std::cerr << "ERROR: PLY file contained an invalid vertex: " << vertex
                << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  for (const auto& normal : context.GetNormals()) {
    if (!VectorValidate(normal)) {
      std::cerr << "ERROR: PLY file contained an invalid normal: " << normal
                << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  for (const auto& texture_coordinate : context.GetUVs()) {
    if (!isfinite(texture_coordinate.first)) {
      std::cerr << "ERROR: PLY file contained an invalid u: "
                << texture_coordinate.first << std::endl;
      exit(EXIT_FAILURE);
    }

    if (!isfinite(texture_coordinate.second)) {
      std::cerr << "ERROR: PLY file contained an invalid v: "
                << texture_coordinate.second << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  for (size_t i = 0; i < context.GetFaces().size(); i += 3) {
    size_t face0 = context.GetFaces()[i];
    size_t face1 = context.GetFaces()[i + 1];
    size_t face2 = context.GetFaces()[i + 2];

    if (face0 == face1 || face1 == face2 || face0 == face2) {
      std::cerr << "ERROR: PLY file contained a 'face' element which used an "
                   "index in 'vertex_indices' more than once"
                << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  return context;
}

}  // namespace

ShapeResult ParsePlyMesh(const char* base_type_name, const char* type_name,
                         Tokenizer& tokenizer, const Material& front_material,
                         const Material& back_material,
                         const EmissiveMaterial& front_emissive_material,
                         const EmissiveMaterial& back_emissive_material) {
  SingleStringMatcher filename(base_type_name, type_name, "filename", true,
                               kPlyMeshDefaultFilename);
  MatchParameters<1>(base_type_name, type_name, tokenizer, {&filename});

  PlyData fileData = ReadPlyFile(filename.Get());

  std::vector<Shape> shapes(fileData.GetFaces().size());
  size_t triangles_allocated;
  ISTATUS status = TriangleMeshAllocate(
      fileData.GetVertices().data(),
      fileData.GetNormals().empty() ? nullptr : fileData.GetNormals().data(),
      fileData.GetUVs().empty()
          ? nullptr
          : reinterpret_cast<const float_t(*)[2]>(fileData.GetUVs().data()),
      fileData.GetVertices().size(),
      reinterpret_cast<const size_t(*)[3]>(fileData.GetFaces().data()),
      fileData.GetFaces().size(), front_material.get(), back_material.get(),
      front_emissive_material.get(), back_emissive_material.get(),
      reinterpret_cast<PSHAPE*>(shapes.data()), &triangles_allocated);

  switch (status) {
    case ISTATUS_ALLOCATION_FAILED:
      ReportOOM();
    default:
      assert(status == ISTATUS_SUCCESS);
  }

  if (triangles_allocated != fileData.GetFaces().size()) {
    std::cerr << "WARNING: PlyMesh contained degenerate triangles that "
                 "were ignored."
              << std::endl;
  }

  std::vector<Light> lights;
  if (!front_emissive_material.get() && !back_emissive_material.get()) {
    return std::make_pair(std::move(shapes), std::move(lights));
  }

  for (size_t i = 0; i < triangles_allocated; i++) {
    if (front_emissive_material.get()) {
      Light light;
      status = AreaLightAllocate(shapes[i].get(), TRIANGLE_MESH_FRONT_FACE,
                                 light.release_and_get_address());
      SuccessOrOOM(status);
      lights.push_back(light);
    }

    if (back_emissive_material.get()) {
      Light light;
      status = AreaLightAllocate(shapes[i].get(), TRIANGLE_MESH_BACK_FACE,
                                 light.release_and_get_address());
      SuccessOrOOM(status);
      lights.push_back(light);
    }
  }

  return std::make_pair(std::move(shapes), std::move(lights));
}

}  // namespace iris
#ifndef _SRC_COMMON_POINTER_TYPES_
#define _SRC_COMMON_POINTER_TYPES_

#include "iris_camera/iris_camera.h"
#include "iris_physx/iris_physx.h"
#include "iris_physx_toolkit/float_texture.h"
#include "iris_physx_toolkit/reflector_texture.h"
#include "src/common/shared_ptr.h"
#include "src/common/unique_ptr.h"

namespace iris {

typedef SharedPtr<BSDF, BsdfRetain, BsdfRelease> Bsdf;
typedef UniquePtr<CAMERA, CameraFree> Camera;
typedef UniquePtr<COLOR_INTEGRATOR, ColorIntegratorFree> ColorIntegrator;
typedef SharedPtr<EMISSIVE_MATERIAL, EmissiveMaterialRetain,
                  EmissiveMaterialRelease>
    EmissiveMaterial;
typedef SharedPtr<FLOAT_TEXTURE, FloatTextureRetain, FloatTextureRelease>
    FloatTexture;
typedef UniquePtr<FRAMEBUFFER, FramebufferFree> Framebuffer;
typedef UniquePtr<INTEGRATOR, IntegratorFree> Integrator;
typedef SharedPtr<MATERIAL, MaterialRetain, MaterialRelease> Material;
typedef SharedPtr<MATRIX, MatrixRetain, MatrixRelease> Matrix;
typedef SharedPtr<LIGHT, LightRetain, LightRelease> Light;
typedef UniquePtr<LIGHT_SAMPLER, LightSamplerFree> LightSampler;
typedef UniquePtr<PIXEL_SAMPLER, PixelSamplerFree> PixelSampler;
typedef UniquePtr<RANDOM, RandomFree> Random;
typedef SharedPtr<REFLECTOR, ReflectorRetain, ReflectorRelease> Reflector;
typedef SharedPtr<REFLECTOR_TEXTURE, ReflectorTextureRetain,
                  ReflectorTextureRelease>
    ReflectorTexture;
typedef UniquePtr<SAMPLE_TRACER, SampleTracerFree> SampleTracer;
typedef UniquePtr<SCENE, SceneFree> Scene;
typedef SharedPtr<SHAPE, ShapeRetain, ShapeRelease> Shape;
typedef SharedPtr<SPECTRUM, SpectrumRetain, SpectrumRelease> Spectrum;

}  // namespace iris

#endif  // _SRC_COMMON_POINTER_TYPES_
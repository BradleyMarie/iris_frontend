#ifndef _SRC_COMMON_POINTER_TYPES_
#define _SRC_COMMON_POINTER_TYPES_

#include "iris_advanced_toolkit/low_discrepancy_sequence.h"
#include "iris_camera/iris_camera.h"
#include "iris_physx/iris_physx.h"
#include "iris_physx_toolkit/color_extrapolator.h"
#include "iris_physx_toolkit/float_texture.h"
#include "iris_physx_toolkit/mipmap.h"
#include "iris_physx_toolkit/reflector_texture.h"
#include "src/common/shared_ptr.h"
#include "src/common/unique_ptr.h"

namespace iris {

typedef SharedPtr<BSDF, BsdfRetain, BsdfRelease> Bsdf;
typedef UniquePtr<CAMERA, CameraFree> Camera;
typedef UniquePtr<COLOR_EXTRAPOLATOR, ColorExtrapolatorFree> ColorExtrapolator;
typedef SharedPtr<COLOR_INTEGRATOR, ColorIntegratorRetain,
                  ColorIntegratorRelease>
    ColorIntegrator;
typedef SharedPtr<EMISSIVE_MATERIAL, EmissiveMaterialRetain,
                  EmissiveMaterialRelease>
    EmissiveMaterial;
typedef UniquePtr<FLOAT_MIPMAP, FloatMipmapFree> FloatMipmap;
typedef SharedPtr<FLOAT_TEXTURE, FloatTextureRetain, FloatTextureRelease>
    FloatTexture;
typedef UniquePtr<FRAMEBUFFER, FramebufferFree> Framebuffer;
typedef UniquePtr<INTEGRATOR, IntegratorFree> Integrator;
typedef SharedPtr<MATERIAL, MaterialRetain, MaterialRelease> Material;
typedef SharedPtr<MATRIX, MatrixRetain, MatrixRelease> Matrix;
typedef SharedPtr<NORMAL_MAP, NormalMapRetain, NormalMapRelease> NormalMap;
typedef SharedPtr<LIGHT, LightRetain, LightRelease> Light;
typedef SharedPtr<LIGHT_SAMPLER, LightSamplerRetain, LightSamplerRelease>
    LightSampler;
typedef UniquePtr<LOW_DISCREPANCY_SEQUENCE, LowDiscrepancySequenceFree>
    LowDiscrepancySequence;
typedef UniquePtr<PROGRESS_REPORTER, ProgressReporterFree> ProgressReporter;
typedef UniquePtr<RANDOM, RandomFree> Random;
typedef SharedPtr<REFLECTOR, ReflectorRetain, ReflectorRelease> Reflector;
typedef UniquePtr<REFLECTOR_MIPMAP, ReflectorMipmapFree> ReflectorMipmap;
typedef SharedPtr<REFLECTOR_TEXTURE, ReflectorTextureRetain,
                  ReflectorTextureRelease>
    ReflectorTexture;
typedef UniquePtr<IMAGE_SAMPLER, ImageSamplerFree> Sampler;
typedef UniquePtr<SAMPLE_TRACER, SampleTracerFree> SampleTracer;
typedef SharedPtr<SCENE, SceneRetain, SceneRelease> Scene;
typedef SharedPtr<SHAPE, ShapeRetain, ShapeRelease> Shape;
typedef SharedPtr<SPECTRUM, SpectrumRetain, SpectrumRelease> Spectrum;

}  // namespace iris

#endif  // _SRC_COMMON_POINTER_TYPES_
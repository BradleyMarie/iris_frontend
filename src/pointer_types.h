#include "iris_camera/iris_camera.h"
#include "iris_physx/iris_physx.h"
#include "src/shared_ptr.h"
#include "src/unique_ptr.h"

namespace iris {

typedef SharedPtr<BSDF, BsdfRetain, BsdfRelease> Bsdf;
typedef UniquePtr<CAMERA, CameraFree> Camera;
typedef UniquePtr<COLOR_INTEGRATOR, ColorIntegratorFree> ColorIntegrator;
typedef SharedPtr<EMISSIVE_MATERIAL, EmissiveMaterialRetain,
                  EmissiveMaterialRelease>
    EmissiveMaterial;
typedef UniquePtr<FRAMEBUFFER, FramebufferFree> Framebuffer;
typedef UniquePtr<INTEGRATOR, IntegratorFree> Integrator;
typedef SharedPtr<MATERIAL, MaterialRetain, MaterialRelease> Material;
typedef SharedPtr<MATRIX, MatrixRetain, MatrixRelease> Matrix;
typedef SharedPtr<LIGHT, LightRetain, LightRelease> Light;
typedef UniquePtr<RANDOM, RandomFree> Random;
typedef SharedPtr<REFLECTOR, ReflectorRetain, ReflectorRelease> Reflector;
typedef SharedPtr<SPECTRUM, SpectrumRetain, SpectrumRelease> Spectrum;

}  // namespace iris
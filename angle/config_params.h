#ifndef ANGLE_CONFIG_PARAMS
#define ANGLE_CONFIG_PARAMS 

#include "base/optional.h"
#include "angle/egl_loader_autogen.h"
#include "angle/gles_loader_autogen.h"

namespace angle {

extern EGLint kDefaultSwapInterval;

struct ConfigParameters {
    ConfigParameters();
    ~ConfigParameters();

    void Reset();

    // Surface and Context parameters.
    int red_bits;
    int green_bits;
    int blue_bits;
    int alpha_bits;
    int depth_bits;
    int stencil_bits;

    Optional<bool> webgl_compatibility;
    Optional<bool> robust_resource_init;

    // EGLWindow-specific.
    EGLenum component_type;
    bool multisample;
    bool debug;
    bool no_error;
    Optional<bool> extensions_enabled;
    bool bind_generates_resource;
    bool client_arrays_enabled;
    bool robust_access;
    bool mutable_render_buffer;
    EGLint samples;
    Optional<bool> context_program_cache_enabled;
    EGLenum reset_strategy;
    EGLenum color_space;
    EGLint swap_interval;
};

} // namespace angle
#endif // ANGLE_CONFIG_PARAMS
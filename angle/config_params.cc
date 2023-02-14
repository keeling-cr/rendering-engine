#include "angle/config_params.h"

namespace angle {

EGLint kDefaultSwapInterval = 1;

ConfigParameters::ConfigParameters()
    : red_bits(-1),
      green_bits(-1),
      blue_bits(-1),
      alpha_bits(-1),
      depth_bits(-1),
      stencil_bits(-1),
      component_type(EGL_COLOR_COMPONENT_TYPE_FIXED_EXT),
      multisample(false),
      debug(false),
      no_error(false),
      bind_generates_resource(true),
      client_arrays_enabled(true),
      robust_access(false),
      mutable_render_buffer(false),
      samples(-1),
      reset_strategy(EGL_NO_RESET_NOTIFICATION_EXT),
      color_space(EGL_COLORSPACE_LINEAR),
      swap_interval(kDefaultSwapInterval)
{}

ConfigParameters::~ConfigParameters() = default;

void ConfigParameters::Reset()
{
    *this = ConfigParameters();
}

} // namespace angle
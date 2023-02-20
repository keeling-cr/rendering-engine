#include "angle/egl_window.h"

#include <iostream>
#include <string.h>
#include <vector>

#include "base/system/system_utils.h"
#include "base/logging.h"
#include "angle/os_window.h"

namespace angle {

// static
void EGLWindow::Delete(EGLWindow** window) {
    delete *window;
    *window = nullptr;
}

// static
EGLWindow *EGLWindow::New(EGLenum client_type,
                          EGLint gles_major_version,
                          EGLint gles_minor_version,
                          EGLint profile_mask) {
    return new EGLWindow(client_type, gles_major_version, gles_minor_version, profile_mask);
}

EGLWindow::EGLWindow(EGLenum client_type,
                    EGLint gles_major_version,
                    EGLint gles_minor_version,
                    EGLint profile_mask)
    :config_(0),
     client_major_version_(gles_major_version),
     client_minor_version_(gles_minor_version),
     profile_mask_(profile_mask),
     client_type_(client_type),
     display_(EGL_NO_DISPLAY),
     surface_(EGL_NO_SURFACE),
     context_(EGL_NO_CONTEXT),
     egl_major_version_(0),
     egl_minor_version_(0) {}

EGLWindow::~EGLWindow() {
    // todo(liqining)
    // destroyGL();
}

EGLConfig EGLWindow::GetConfig() const {
    return config_;
}

EGLDisplay EGLWindow::GetDisplay() const {
    return display_;
}

EGLSurface EGLWindow::GetSurface() const {
    return surface_;
}

EGLContext EGLWindow::GetContext() const {
    return context_;
}

bool EGLWindow::SetSwapInterval(EGLint swap_interval) {
    if (eglSwapInterval(display_, swap_interval) == EGL_FALSE || eglGetError() != EGL_SUCCESS) {
        fprintf(stderr, "Error during eglSwapInterval.\n");
        return false;
    }
    return true;
}

bool EGLWindow::InitializeGL(OSWindow* os_window,
        base::Library* gl_window_library,
        const ConfigParameters& config_params) {
    return InitializeGLWithResult(
        os_window, gl_window_library, config_params) == GLWindowResult::NoError;
}

GLWindowResult 
EGLWindow::InitializeGLWithResult(OSWindow *os_window,
                            base::Library* gl_window_library,
                            const ConfigParameters& config_params) {
    if (!InitializeDisplay(gl_window_library, os_window)) {
        return GLWindowResult::Error;
    }
    GLWindowResult res = InitializeSurface(os_window, gl_window_library, config_params);
    if (res != GLWindowResult::NoError) {
        return res;
    }

    if (!InitializeContext()) {
        return GLWindowResult::Error;
    }
    return GLWindowResult::NoError;
}

bool EGLWindow::InitializeDisplay(
    base::Library* library,
    OSWindow* window) {
    PFNEGLGETPROCADDRESSPROC getProcAddress;
    library->GetAs("eglGetProcAddress", &getProcAddress);
    if (!getProcAddress) {
        fprintf(stderr, "Cannot load eglGetProcAddress\n");
        return false;
    }

    LoadUtilEGL(getProcAddress);

    const char *extension =
        static_cast<const char *>(eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS));
    if (!extension) {
        extension = "";
    }
    
    std::vector<EGLAttrib> display_attributes;
    display_attributes.push_back(EGL_PLATFORM_ANGLE_TYPE_ANGLE);
    display_attributes.push_back(EGL_PLATFORM_ANGLE_TYPE_DEFAULT_ANGLE);
    display_attributes.push_back(EGL_PLATFORM_ANGLE_MAX_VERSION_MAJOR_ANGLE);
    display_attributes.push_back(EGL_DONT_CARE);
    display_attributes.push_back(EGL_PLATFORM_ANGLE_MAX_VERSION_MINOR_ANGLE);
    display_attributes.push_back(EGL_DONT_CARE);
    display_attributes.push_back(EGL_PLATFORM_ANGLE_DEVICE_TYPE_ANGLE);
    display_attributes.push_back(EGL_PLATFORM_ANGLE_DEVICE_TYPE_HARDWARE_ANGLE);
    display_attributes.push_back(EGL_NONE);

    display_ = eglGetPlatformDisplay(EGL_PLATFORM_ANGLE_ANGLE,
                                    reinterpret_cast<void*>(EGL_DEFAULT_DISPLAY),
                                    &display_attributes[0]);
    
    if (display_ == EGL_NO_DISPLAY) {
        fprintf(stderr, "Failed to get display: 0x%X\n", eglGetError());
        // todo(liqining)
        // destroyGL();
        return false;
    }

    if (eglInitialize(display_, &egl_major_version_, &egl_minor_version_) == EGL_FALSE) {
        fprintf(stderr, "eglInitialize failed: 0x%X\n", eglGetError());
        // todo(liqining)
        // destroyGL();
        return false;
    }
    return true;
}

GLWindowResult EGLWindow::InitializeSurface(OSWindow *window,
                                            base::Library* gl_window_library,
                                            const ConfigParameters &params)  {
    config_params_                 = params;
    const char *display_extensions = eglQueryString(display_, EGL_EXTENSIONS);

    bool has_mutable_render_buffer =
        strstr(display_extensions, "EGL_KHR_mutable_render_buffer") != nullptr;
    if (config_params_.mutable_render_buffer && !has_mutable_render_buffer) {
        LOG(ERROR) << "Mising EGL_KHR_mutable_render_buffer.";
        // destroyGL();
        return GLWindowResult::NoMutableRenderBufferSupport;
    }

    std::vector<EGLint> config_attributes = {
        EGL_SURFACE_TYPE,
        EGL_WINDOW_BIT | (params.mutable_render_buffer ? EGL_MUTABLE_RENDER_BUFFER_BIT_KHR : 0),
        EGL_RED_SIZE,
        (config_params_.red_bits >= 0) ? config_params_.red_bits : EGL_DONT_CARE,
        EGL_GREEN_SIZE,
        (config_params_.green_bits >= 0) ? config_params_.green_bits : EGL_DONT_CARE,
        EGL_BLUE_SIZE,
        (config_params_.blue_bits >= 0) ? config_params_.blue_bits : EGL_DONT_CARE,
        EGL_ALPHA_SIZE,
        (config_params_.alpha_bits >= 0) ? config_params_.alpha_bits : EGL_DONT_CARE,
        EGL_DEPTH_SIZE,
        (config_params_.depth_bits >= 0) ? config_params_.depth_bits : EGL_DONT_CARE,
        EGL_STENCIL_SIZE,
        (config_params_.stencil_bits >= 0) ? config_params_.stencil_bits : EGL_DONT_CARE,
        EGL_SAMPLE_BUFFERS,
        config_params_.multisample ? 1 : 0,
        EGL_SAMPLES,
        (config_params_.samples >= 0) ? config_params_.samples : EGL_DONT_CARE,
    };

    // Add dynamic attributes
    bool has_pixel_format_float = strstr(display_extensions, "EGL_EXT_pixel_format_float") != nullptr;
    if (!has_pixel_format_float && config_params_.component_type != EGL_COLOR_COMPONENT_TYPE_FIXED_EXT) {
        LOG(ERROR) << "Mising EGL_EXT_pixel_format_float";
        // destroyGL();
        return GLWindowResult::Error;
    }
    if (has_pixel_format_float) {
        config_attributes.push_back(EGL_COLOR_COMPONENT_TYPE_EXT);
        config_attributes.push_back(config_params_.component_type);
    }

    // Finish the attribute list
    config_attributes.push_back(EGL_NONE);

    if (!FindEGLConfig(display_, config_attributes.data(), &config_)) {
        LOG(ERROR) << "Could not find a suitable EGL config!";
        // destroyGL();
        return GLWindowResult::Error;
    }

    eglGetConfigAttrib(display_, config_, EGL_RED_SIZE, &config_params_.red_bits);
    eglGetConfigAttrib(display_, config_, EGL_GREEN_SIZE, &config_params_.green_bits);
    eglGetConfigAttrib(display_, config_, EGL_BLUE_SIZE, &config_params_.blue_bits);
    eglGetConfigAttrib(display_, config_, EGL_ALPHA_SIZE, &config_params_.alpha_bits);
    eglGetConfigAttrib(display_, config_, EGL_DEPTH_SIZE, &config_params_.depth_bits);
    eglGetConfigAttrib(display_, config_, EGL_STENCIL_SIZE, &config_params_.stencil_bits);
    eglGetConfigAttrib(display_, config_, EGL_SAMPLES, &config_params_.samples);

    std::vector<EGLint> surface_attributes;
    if (strstr(display_extensions, "EGL_NV_post_sub_buffer") != nullptr) {
        surface_attributes.push_back(EGL_POST_SUB_BUFFER_SUPPORTED_NV);
        surface_attributes.push_back(EGL_TRUE);
    }

    bool has_robust_resource_init =
        strstr(display_extensions, "EGL_ANGLE_robust_resource_initialization") != nullptr;
    if (has_robust_resource_init && config_params_.robust_resource_init.valid()) {
        surface_attributes.push_back(EGL_ROBUST_RESOURCE_INITIALIZATION_ANGLE);
        surface_attributes.push_back(config_params_.robust_resource_init.value() ? EGL_TRUE
                                                                             : EGL_FALSE);
    }

    bool has_gl_color_space = strstr(display_extensions, "EGL_KHR_gl_colorspace") != nullptr;
    if (!has_gl_color_space && config_params_.color_space != EGL_COLORSPACE_LINEAR) {
        LOG(ERROR) << "Mising EGL_KHR_gl_colorspace.";
        // destroyGL();
        return GLWindowResult::NoColorspaceSupport;
    }

    if (has_gl_color_space) {
        surface_attributes.push_back(EGL_GL_COLORSPACE_KHR);
        surface_attributes.push_back(config_params_.color_space);
    }

    bool has_create_surface_swap_interval =
        strstr(display_extensions, "EGL_ANGLE_create_surface_swap_interval") != nullptr;
    if (has_create_surface_swap_interval && config_params_.swap_interval != kDefaultSwapInterval) {
        surface_attributes.push_back(EGL_SWAP_INTERVAL_ANGLE);
        surface_attributes.push_back(config_params_.swap_interval);
    }

    surface_attributes.push_back(EGL_NONE);

    window->ResetNativeWindow();

    surface_ = eglCreateWindowSurface(display_, config_, window->GetNativeWindow(),
                                      &surface_attributes[0]);
    if (eglGetError() != EGL_SUCCESS || (surface_ == EGL_NO_SURFACE))
    {
        fprintf(stderr, "eglCreateWindowSurface failed: 0x%X\n", eglGetError());
        // destroyGL();
        return GLWindowResult::Error;
    }

    LoadUtilGLES(eglGetProcAddress);

    return GLWindowResult::NoError;
}

EGLContext EGLWindow::CreateContext(EGLContext share, EGLint *extra_attributes) {
    const char *display_extensions = eglQueryString(display_, EGL_EXTENSIONS);

    // EGL_KHR_create_context is required to request a ES3+ context.
    bool has_khr_create_context = strstr(display_extensions, "EGL_KHR_create_context") != nullptr;
    if (client_major_version_ > 2 && !(egl_major_version_ > 1 || egl_minor_version_ >= 5) &&
        !has_khr_create_context) {
        fprintf(stderr, "EGL_KHR_create_context incompatibility.\n");
        return EGL_NO_CONTEXT;
    }

    // EGL_CONTEXT_OPENGL_DEBUG is only valid as of EGL 1.5.
    bool has_debug = egl_minor_version_ >= 5;
    if (config_params_.debug && !has_debug) {
        fprintf(stderr, "EGL 1.5 is required for EGL_CONTEXT_OPENGL_DEBUG.\n");
        return EGL_NO_CONTEXT;
    }

    bool has_web_gl_compatibility =
        strstr(display_extensions, "EGL_ANGLE_create_context_webgl_compatibility") != nullptr;
    if (config_params_.webgl_compatibility.valid() && !has_web_gl_compatibility) {
        fprintf(stderr, "EGL_ANGLE_create_context_webgl_compatibility missing.\n");
        return EGL_NO_CONTEXT;
    }

    bool has_create_context_extensions_enabled =
        strstr(display_extensions, "EGL_ANGLE_create_context_extensions_enabled") != nullptr;
    if (config_params_.extensions_enabled.valid() && !has_create_context_extensions_enabled) {
        fprintf(stderr, "EGL_ANGLE_create_context_extensions_enabled missing.\n");
        return EGL_NO_CONTEXT;
    }

    bool has_robustness = strstr(display_extensions, "EGL_EXT_create_context_robustness") != nullptr;
    if ((config_params_.robust_access ||
         config_params_.reset_strategy != EGL_NO_RESET_NOTIFICATION_EXT) &&
        !has_robustness) {
        fprintf(stderr, "EGL_EXT_create_context_robustness missing.\n");
        return EGL_NO_CONTEXT;
    }

    bool has_bind_generates_resource =
        strstr(display_extensions, "EGL_CHROMIUM_create_context_bind_generates_resource") != nullptr;
    if (!config_params_.bind_generates_resource && !has_bind_generates_resource) {
        fprintf(stderr, "EGL_CHROMIUM_create_context_bind_generates_resource missing.\n");
        return EGL_NO_CONTEXT;
    }

    bool has_client_arrays_extension =
        strstr(display_extensions, "EGL_ANGLE_create_context_client_arrays") != nullptr;
    if (!config_params_.client_arrays_enabled && !has_client_arrays_extension) {
        // Non-default state requested without the extension present
        fprintf(stderr, "EGL_ANGLE_create_context_client_arrays missing.\n");
        return EGL_NO_CONTEXT;
    }

    bool has_program_cache_control_extension =
        strstr(display_extensions, "EGL_ANGLE_program_cache_control ") != nullptr;
    if (config_params_.context_program_cache_enabled.valid() && !has_program_cache_control_extension) {
        fprintf(stderr, "EGL_ANGLE_program_cache_control missing.\n");
        return EGL_NO_CONTEXT;
    }

    bool has_khr_create_context_on_error =
        strstr(display_extensions, "EGL_KHR_create_context_no_error") != nullptr;
    if (config_params_.no_error && !has_khr_create_context_on_error) {
        fprintf(stderr, "EGL_KHR_create_context_no_error missing.\n");
        return EGL_NO_CONTEXT;
    }

    eglBindAPI(client_type_);
    if (eglGetError() != EGL_SUCCESS) {
        fprintf(stderr, "Error on eglBindAPI.\n");
        return EGL_NO_CONTEXT;
    }

    std::vector<EGLint> context_attributes;
    for (EGLint *extra_attrib = extra_attributes;
         extra_attrib != nullptr && extra_attrib[0] != EGL_NONE; extra_attrib += 2) {
        context_attributes.push_back(extra_attrib[0]);
        context_attributes.push_back(extra_attrib[1]);
    }

    if (has_khr_create_context) {
        context_attributes.push_back(EGL_CONTEXT_MAJOR_VERSION_KHR);
        context_attributes.push_back(client_major_version_);

        context_attributes.push_back(EGL_CONTEXT_MINOR_VERSION_KHR);
        context_attributes.push_back(client_minor_version_);

        if (profile_mask_ != 0) {
            context_attributes.push_back(EGL_CONTEXT_OPENGL_PROFILE_MASK);
            context_attributes.push_back(profile_mask_);
        }

        if (has_debug && config_params_.debug) {
            context_attributes.push_back(EGL_CONTEXT_OPENGL_DEBUG);
            context_attributes.push_back(config_params_.debug ? EGL_TRUE : EGL_FALSE);
        }

        if (has_khr_create_context_on_error && config_params_.no_error) {
            context_attributes.push_back(EGL_CONTEXT_OPENGL_NO_ERROR_KHR);
            context_attributes.push_back(config_params_.no_error ? EGL_TRUE : EGL_FALSE);
        }

        if (config_params_.webgl_compatibility.valid()) {
            context_attributes.push_back(EGL_CONTEXT_WEBGL_COMPATIBILITY_ANGLE);
            context_attributes.push_back(config_params_.webgl_compatibility.value() ? EGL_TRUE
                                                                                 : EGL_FALSE);
        }

        if (config_params_.extensions_enabled.valid()) {
            context_attributes.push_back(EGL_EXTENSIONS_ENABLED_ANGLE);
            context_attributes.push_back(config_params_.extensions_enabled.value() ? EGL_TRUE
                                                                                : EGL_FALSE);
        }

        if (has_robustness) {
            context_attributes.push_back(EGL_CONTEXT_OPENGL_ROBUST_ACCESS_EXT);
            context_attributes.push_back(config_params_.robust_access ? EGL_TRUE : EGL_FALSE);

            context_attributes.push_back(EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_EXT);
            context_attributes.push_back(config_params_.reset_strategy);
        }

        if (has_bind_generates_resource) {
            context_attributes.push_back(EGL_CONTEXT_BIND_GENERATES_RESOURCE_CHROMIUM);
            context_attributes.push_back(config_params_.bind_generates_resource ? EGL_TRUE : EGL_FALSE);
        }

        if (has_client_arrays_extension) {
            context_attributes.push_back(EGL_CONTEXT_CLIENT_ARRAYS_ENABLED_ANGLE);
            context_attributes.push_back(config_params_.client_arrays_enabled ? EGL_TRUE : EGL_FALSE);
        }

        if (config_params_.context_program_cache_enabled.valid()) {
            context_attributes.push_back(EGL_CONTEXT_PROGRAM_BINARY_CACHE_ENABLED_ANGLE);
            context_attributes.push_back(
                config_params_.context_program_cache_enabled.value() ? EGL_TRUE : EGL_FALSE);
        }

        bool has_backwards_compatible_context_extension =
            strstr(display_extensions, "EGL_ANGLE_create_context_backwards_compatible") != nullptr;
        if (has_backwards_compatible_context_extension) {
            context_attributes.push_back(EGL_CONTEXT_OPENGL_BACKWARDS_COMPATIBLE_ANGLE);
            context_attributes.push_back(EGL_FALSE);
        }

        bool has_robust_resource_init =
            strstr(display_extensions, "EGL_ANGLE_robust_resource_initialization") != nullptr;
        if (has_robust_resource_init && config_params_.robust_resource_init.valid()) {
            context_attributes.push_back(EGL_ROBUST_RESOURCE_INITIALIZATION_ANGLE);
            context_attributes.push_back(config_params_.robust_resource_init.value() ? EGL_TRUE
                                                                                 : EGL_FALSE);
        }
    }
    context_attributes.push_back(EGL_NONE);

    EGLContext context = eglCreateContext(display_, config_, share, &context_attributes[0]);
    if (context == EGL_NO_CONTEXT) {
        fprintf(stderr, "eglCreateContext failed: 0x%X\n", eglGetError());
        return EGL_NO_CONTEXT;
    }

    return context;
}

EGLBoolean EGLWindow::FindEGLConfig(EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *config) {
    EGLint num_configs = 0;
    eglGetConfigs(dpy, nullptr, 0, &num_configs);
    std::vector<EGLConfig> all_configs(num_configs);
    eglGetConfigs(dpy, all_configs.data(), static_cast<EGLint>(all_configs.size()), &num_configs);

    for (size_t i = 0; i < all_configs.size(); i++) {
        bool match_found = true;
        for (const EGLint *cur_attrib = attrib_list; cur_attrib[0] != EGL_NONE; cur_attrib += 2) {
            if (cur_attrib[1] == EGL_DONT_CARE) {
                continue;
            }

            EGLint actual_value = EGL_DONT_CARE;
            eglGetConfigAttrib(dpy, all_configs[i], cur_attrib[0], &actual_value);
            if ((cur_attrib[0] == EGL_SURFACE_TYPE &&
                 (cur_attrib[1] & actual_value) != cur_attrib[1]) ||
                (cur_attrib[0] != EGL_SURFACE_TYPE && cur_attrib[1] != actual_value)) {
                match_found = false;
                break;
            }
        }

        if (match_found) {
            *config = all_configs[i];
            return EGL_TRUE;
        }
    }

    return EGL_FALSE;
}

bool EGLWindow::MakeCurrent(){
    return MakeCurrent(context_);
}

bool EGLWindow::MakeCurrent(EGLContext context){
    return MakeCurrent(surface_, surface_, context);
}

bool EGLWindow::MakeCurrent(EGLSurface draw, EGLSurface read, EGLContext context){
    if ((draw && !read) || (!draw && read)) {
        fprintf(stderr, "eglMakeCurrent: setting only one of draw and read buffer is illegal\n");
        return false;
    }

    if (!draw) {
        draw = read = context != EGL_NO_CONTEXT ? surface_ : EGL_NO_SURFACE;
    }

    if (IsGLInitialized()) {
        if (eglMakeCurrent(display_, draw, read, context) == EGL_FALSE ||
            eglGetError() != EGL_SUCCESS) {
            fprintf(stderr, "Error during eglMakeCurrent.\n");
            return false;
        }
    }
    return true;
}

bool EGLWindow::IsGLInitialized() const {
    return surface_ != EGL_NO_SURFACE && context_ != EGL_NO_CONTEXT && display_ != EGL_NO_DISPLAY;
}

bool EGLWindow::InitializeContext() {
    context_ = CreateContext(EGL_NO_CONTEXT, nullptr);
    if (context_ == EGL_NO_CONTEXT) {
        // destroyGL();
        return false;
    }

    if (!MakeCurrent()) {
        // destroyGL();
        return false;
    }

    return true;
}

void EGLWindow::Swap() {
    eglSwapBuffers(display_, surface_);
}

} // namespace angle
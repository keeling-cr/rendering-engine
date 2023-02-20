#ifndef ANGLE_ELG_WINDOW_H
#define ANGLE_ELG_WINDOW_H 

#include "angle/egl_loader_autogen.h"
#include "base/system/system_utils.h"
#include "angle/config_params.h"
namespace angle {

class Library;
class OSWindow;


enum class GLWindowResult {
    NoError,
    NoColorspaceSupport,
    NoMutableRenderBufferSupport,
    Error,
};

class EGLWindow {
    public:
        static EGLWindow* New(EGLenum client_type, 
                            EGLint gles_major_version,
                            EGLint gles_minor_version,
                            EGLint profile_mask);
        
        static void Delete(EGLWindow** window);
        
        static EGLBoolean FindEGLConfig(
            EGLDisplay dpy, const EGLint* attrib_list, EGLConfig* config);

        EGLConfig GetConfig() const;
        EGLDisplay GetDisplay() const;
        EGLSurface GetSurface() const;
        EGLContext GetContext() const;

        bool SetSwapInterval(EGLint swap_interval);

        bool MakeCurrent();
        bool MakeCurrent(EGLSurface draw, EGLSurface read, EGLContext context);
        void Swap();
        
        bool InitializeGL(OSWindow* os_window,
                      base::Library* gl_window_library,
                      const ConfigParameters& config_params);

        GLWindowResult InitializeGLWithResult(OSWindow *osWindow,
                        base::Library *glWindowingLibrary,
                        const ConfigParameters &config_params);

        bool InitializeDisplay(
            base::Library* gl_window_library,
            OSWindow* os_window);
        GLWindowResult InitializeSurface(
            OSWindow* window, base::Library* gl_window_library, const ConfigParameters& params);
        bool InitializeContext(); 

        bool IsGLInitialized() const;
    private:
        EGLWindow(
            EGLenum client_type, 
            EGLint gles_major_version, 
            EGLint gles_minor_version, 
            EGLint profile_mask);
        ~EGLWindow();

        EGLContext CreateContext(EGLContext share, EGLint *extraAttributes);
        bool MakeCurrent(EGLContext context);

        EGLenum client_type_;
        EGLint client_major_version_;
        EGLint client_minor_version_;
        EGLint profile_mask_;

        ConfigParameters config_params_;
        EGLConfig config_;
        EGLDisplay display_;
        EGLSurface surface_;
        EGLContext context_;

        EGLint egl_major_version_;
        EGLint egl_minor_version_;
};       

} // namespace angle

#endif // ANGLE_ELG_WINDOW_H
#ifndef ANGLE_ANGLE_UTIL_H_
#define ANGLE_ANGLE_UTIL_H_
#include "base/system/system_utils.h"

namespace angle {

namespace {
const char* kAngleLibPath = "/Users/bytedance/rendering-engine/third_party/angle/libs/libEGL.dylib";

base::Library* g_angle_egl_library = nullptr;
} // namespace

base::Library* AngleEGLLibrary() {
    if (!g_angle_egl_library) 
        g_angle_egl_library = base::OpenSharedLibrary(kAngleLibPath);  
    return g_angle_egl_library;
}

} // namespace angle
#endif // ANGLE_ANGLE_UTIL_H_ 
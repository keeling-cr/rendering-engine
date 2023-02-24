#ifndef PLATFORM_WINDOW_UTIL_H_
#define PLATFORM_WINDOW_UTIL_H_

#include "platform/window/os_window.h"
namespace platform { 

namespace {

OSWindow* g_os_window = nullptr;

}

inline bool InitPlatformWindow(const std::string& name, int width, int height) {
    if (g_os_window)
        return true;
    
    g_os_window = OSWindow::New();
    if(!g_os_window->Initialize(name, width, height))
        return false;
    g_os_window->SetVisible(true);

    return true;
}

//todo(liqining): multi-thread security!!!
inline OSWindow* GetPlatformWindow() {
    return g_os_window;
}

} // namespace platform
#endif // PLATFORM_WINDOW_UTIL_H_
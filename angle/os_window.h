#ifndef ANGLE_OS_WINDOW_H_
#define ANGLE_OS_WINDOW_H_

#include <stdint.h>
#include <list>
#include <string>

#include <EGL/egl.h>
#include <EGL/eglext.h>

namespace angle {

class OSWindow {
  public:
    static OSWindow *New();
    static void Delete(OSWindow** os_window);

    bool Initialize(const std::string &name, int width, int height);
    virtual void Destroy()                   = 0;

    int GetX() const;
    int GetY() const;
    int GetWidth() const;
    int GetHeight() const;

    virtual void SetVisible(bool visible)            = 0;
    virtual void MessageLoop() = 0;
    virtual void ResetNativeWindow() = 0;
    virtual EGLNativeWindowType GetNativeWindow() const = 0;
    virtual void SetNativeDisplay(EGLNativeDisplayType display) {}
    virtual EGLNativeDisplayType GetNativeDisplay() const = 0;

    // Whether window has been successfully initialized.
    bool valid() const { return valid_; }

  protected:
    OSWindow();
    virtual ~OSWindow();

    virtual bool InitializeImpl(const std::string &name, int width, int height) = 0;

    int x_;
    int y_;
    int width_;
    int height_;
    bool valid_;
};
} // namespace angle

#endif  // ANGLE_OS_WINDOW_H_

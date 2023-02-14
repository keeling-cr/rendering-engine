#ifndef ANGLE_PLATFORM_OSX_OSX_WINDOW_H_
#define ANGLE_PLATFORM_OSX_OSX_WINDOW_H_

#include "angle/os_window.h"

#ifndef __OBJC__
class NSWindow;
class NSView;
#endif

class OSXWindow : public angle::OSWindow {
  public:
    OSXWindow();
    ~OSXWindow() override;

    void Destroy() override;

    void SetVisible(bool visible) override;
    void MessageLoop() override;
    void ResetNativeWindow() override;
    EGLNativeWindowType GetNativeWindow() const override;
    EGLNativeDisplayType GetNativeDisplay() const override;

    NSWindow *GetNSWindow() const;

  private:
    bool InitializeImpl(const std::string &name, int width, int height) override;

    NSWindow* window_;
    NSView* view_;
};

#endif  // ANGLE_PLATFORM_OSX_OSX_WINDOW_H_

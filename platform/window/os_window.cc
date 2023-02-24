#include "platform/window/os_window.h"
#include "base/logging.h"

namespace platform {

OSWindow::OSWindow() 
    : x_(0), y_(0), width_(0), height_(0), valid_(false) {}

OSWindow::~OSWindow() {}

bool OSWindow::Initialize(const std::string &name, int width, int height) {
    valid_ = InitializeImpl(name, width, height);
    return valid_;
}

int OSWindow::GetX() const {
    return x_;
}

int OSWindow::GetY() const {
    return y_;
}

int OSWindow::GetWidth() const {
    return width_;
}

int OSWindow::GetHeight() const {
    return height_;
}

// static
void OSWindow::Delete(OSWindow** window) {
    delete *window;
    *window = nullptr;
}

} // namespace platform
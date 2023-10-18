#include <iostream>
#include "core/js_runtime.h"
#include "base/logging.h"
#include "base/files/file_path.h"

#include "base/system/system_utils.h"
#include "angle/egl_window.h"
#include "platform/window/util.h"
#include "platform/window/os_window.h"
#include "platform/window/osx/osx_window.h"
#include "angle/config_params.h"
#include "angle/shader_util.h"


int main(int argc, char *argv[]) {
    platform::InitPlatformWindow("nica", 1000, 1000);

    bool running   = true;
    nica::JSRuntime runtime;
    nica::JSRuntime::Scope runtime_scope(&runtime);
    runtime.EvaluateJavascriptSource("'Hello' + ', World!'");

    if (argc > 0) {
        runtime.EvaluateJavascriptFile(base::FilePath(argv[1]));
    }
    platform::OSWindow* window = platform::GetPlatformWindow();
    while (running) {
        window->MessageLoop();
    }
}
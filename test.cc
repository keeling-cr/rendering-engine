#include <iostream>
#include "core/js_runtime.h"
#include "base/logging.h"
#include "base/files/file_path.h"

int main(int argc, char *argv[]) {
    LOG(ERROR) << "keilingnica " << __func__;
    nica::JSRuntime runtime;
    nica::JSRuntime::Scope runtime_scope(&runtime);
    runtime.EvaluateJavascriptSource("'Hello' + ', World!'");
    if (argc > 0) {
        LOG(ERROR) << "keilingnica " << argv[1];
        runtime.EvaluateJavascriptFile(base::FilePath(argv[1]));
    }

    while (true) {}
}
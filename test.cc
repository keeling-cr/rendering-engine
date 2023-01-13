#include <iostream>
#include "core/js_runtime.h"
#include "base/logging.h"

int main() {
    LOG(ERROR) << "keilingnica " << __func__;
    nica::JSRuntime runtime;
    nica::JSRuntime::Scope runtime_scope(&runtime);
    runtime.EvaluateJavascriptSource("'Hello' + ', World!'");
}
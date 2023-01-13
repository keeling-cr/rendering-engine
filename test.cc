#include <iostream>
#include "core/js_runtime.h"
using namespace std;

int main() {
    nica::JSRuntime runtime;
    nica::JSRuntime::Scope runtime_scope(&runtime);
    runtime.EvaluateJavascriptSource("'Hello' + ', World!'");
}
#ifndef BINDING_MODULES_WEBGL_ANY_H_
#define BINDING_MODULES_WEBGL_ANY_H_

#include <stdint.h>
#include "v8.h"

namespace nica {
class ScriptValue;
}

namespace bind {

nica::ScriptValue WebGLAny(v8::Isolate* isolate, bool value);
nica::ScriptValue WebGLAny(v8::Isolate* isolate, int value);
nica::ScriptValue WebGLAny(v8::Isolate* isolate, unsigned value);
nica::ScriptValue WebGLAny(v8::Isolate* isolate, int64_t value);
nica::ScriptValue WebGLAny(v8::Isolate* isolate, uint64_t value);
nica::ScriptValue WebGLAny(v8::Isolate* isolate, float value);
}  // namespace bind

#endif  // BINDING_MODULES_WEBGL_ANY_H_

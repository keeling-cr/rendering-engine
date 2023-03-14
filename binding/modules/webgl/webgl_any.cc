#include "binding/modules/webgl/webgl_any.h"

#include "core/script_value.h"

namespace bind {

nica::ScriptValue WebGLAny(v8::Isolate* isolate, bool value) {
    return nica::ScriptValue(v8::Boolean::New(isolate, value));
}

nica::ScriptValue WebGLAny(v8::Isolate* isolate, int value) {
    return nica::ScriptValue(v8::Integer::New(isolate, value));    
}

nica::ScriptValue WebGLAny(v8::Isolate* isolate, unsigned value) {
    return nica::ScriptValue(
      v8::Integer::NewFromUnsigned(isolate,
                                   static_cast<unsigned>(value)));
}

nica::ScriptValue WebGLAny(v8::Isolate* isolate, int64_t value) {
    return nica::ScriptValue(
        v8::Number::New(isolate, static_cast<double>(value)));
}

nica::ScriptValue WebGLAny(v8::Isolate* isolate, uint64_t value) {
    return nica::ScriptValue(
        v8::Number::New(isolate, static_cast<double>(value)));
}


nica::ScriptValue WebGLAny(v8::Isolate* isolate, float value) {
    return nica::ScriptValue(
        v8::Number::New(isolate, value));
}

} // namespace bind
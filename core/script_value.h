#ifndef CORE_SCRIPT_VALUE_H_
#define CORE_SCRIPT_VALUE_H_

#include "base/memory/scoped_refptr.h"
#include "core/converter.h"
#include "v8.h"

namespace nica {

class ScriptValue {
  public:
    ScriptValue() = default;

    ScriptValue(v8::Local<v8::Value> value)
      : value_(value) {}

    template <typename T>
    ScriptValue(v8::MaybeLocal<T> value)
      : value_(value.IsEmpty() ? v8::Local<T>() : value.ToLocalChecked()) {}

    ScriptValue(const ScriptValue& value) = default;
    v8::Local<v8::Value> data() const { return value_; }

    static ScriptValue CreateNull(v8::Isolate* isolate) {
      return ScriptValue(v8::Null(isolate));
    }
  private:
    v8::Local<v8::Value> value_;
};

template<>
struct Converter<ScriptValue> {
  static v8::Local<v8::Value> ToV8(v8::Isolate* isolate,
                            ScriptValue val) {    
    return val.data();
  }
};

}  // namespace nica

#endif  // CORE_SCRIPT_VALUE_H_

#ifndef CORE_TYPED_ARRAY_ARRAY_BUFFER_H
#define CORE_TYPED_ARRAY_ARRAY_BUFFER_H

#include "base/logging.h"
#include "core/typed_array/array_buffer_contents.h"

namespace nica {

class ArrayBufferBase {
 public:
  ArrayBufferBase() = default;
  ~ArrayBufferBase() = default;

  const ArrayBufferContents* Content() const { return &contents_; }
  ArrayBufferContents* Content() { return &contents_; }

  const void* Data() const { return contents_.Data(); }
  void* Data() { return contents_.Data(); }

  const void* DataMaybeShared() const { return contents_.DataMaybeShared(); }
  void* DataMaybeShared() { return contents_.DataMaybeShared(); }

  size_t ByteLength() const { return contents_.DataLength(); }

  bool IsDetached() const { return is_detached_; }

  void Detach() { is_detached_ = true; }

  bool IsShared() const { return contents_.IsShared(); }

 protected:
  explicit ArrayBufferBase(ArrayBufferContents contents)
      : contents_(std::move(contents)) {}

  ArrayBufferContents contents_;
  bool is_detached_ = false;

};

class ArrayBuffer : public ArrayBufferBase {
  public:
    static ArrayBuffer* Create(ArrayBufferContents contents) {
      return new ArrayBuffer(std::move(contents));
    }

    ArrayBuffer() = default;
    ~ArrayBuffer() { LOG(ERROR) << "todo(liqining): Don't forget release memory";}

    explicit ArrayBuffer(ArrayBufferContents contents)
      : ArrayBufferBase(std::move(contents)) {}

};

template <>
struct Converter<ArrayBuffer> {
  static bool FromV8(
    v8::Isolate* isolate, v8::Local<v8::Value> val, ArrayBuffer* out) {
      if (!val->IsTypedArray())
        return false;
      v8::Local<v8::SharedArrayBuffer> v8_shared_array_buffer =
        val.As<v8::SharedArrayBuffer>();
      LOG(ERROR) << "keilingnica " << __func__;
      ArrayBufferContents contents(v8_shared_array_buffer->GetBackingStore());
      out = ArrayBuffer::Create(contents);
      return true;
    }
};
}

#endif // CORE_TYPED_ARRAY_ARRAY_BUFFER_H

#include "core/js_isolate.h"

namespace nica {

JSIsolate::JSIsolate() {
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator =
        v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    isolate_ = v8::Isolate::New(create_params);
}

JSIsolate::~JSIsolate() {
    isolate_ = nullptr;
}

std::unique_ptr<JSIsolate> JSIsolate::Create() {
    return std::unique_ptr<JSIsolate>(new JSIsolate());
}

} // namespace nica
#include "core/typed_array/array_buffer_contents.h"

#include <cstring>

namespace nica {

ArrayBufferContents::~ArrayBufferContents() = default;
ArrayBufferContents::ArrayBufferContents() = default;

void ArrayBufferContents::Detach() {
  backing_store_.reset();
}

void ArrayBufferContents::Reset() {
  backing_store_.reset();
}

void ArrayBufferContents::Transfer(ArrayBufferContents& other) {
  DCHECK(!IsShared());
  DCHECK(!other.Data());
  other.backing_store_ = std::move(backing_store_);
}

void ArrayBufferContents::ShareWith(ArrayBufferContents& other) {
  DCHECK(IsShared());
  DCHECK(!other.Data());
  other.backing_store_ = backing_store_;
}

}  // namespace nica

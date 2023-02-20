#ifndef CORE_TYPED_ARRAY_ARRAY_BUFFER_CONTENTS_H
#define CORE_TYPED_ARRAY_ARRAY_BUFFER_CONTENTS_H

#include "v8.h"
#include "base/check.h"

namespace nica {

class ArrayBufferContents {
  
 public:
  using DataDeleter = void (*)(void* data, size_t length, void* info);

  enum InitializationPolicy { kZeroInitialize, kDontInitialize };

  enum SharingType {
    kNotShared,
    kShared,
  };

  ArrayBufferContents();
  ~ArrayBufferContents();
  explicit ArrayBufferContents(std::shared_ptr<v8::BackingStore> backing_store)
      : backing_store_(std::move(backing_store)) {}


  void Detach();
  void Reset();

  void* Data() const {
    DCHECK(!IsShared());
    return DataMaybeShared();
  }
  void* DataShared() const {
    DCHECK(IsShared());
    return DataMaybeShared();
  }
  void* DataMaybeShared() const {
    return backing_store_ ? backing_store_->Data() : nullptr;
  }
  size_t DataLength() const {
    return backing_store_ ? backing_store_->ByteLength() : 0;
  }
  bool IsShared() const {
    return backing_store_ ? backing_store_->IsShared() : false;
  }
  bool IsValid() const { return backing_store_ && backing_store_->Data(); }

  std::shared_ptr<v8::BackingStore> BackingStore() const {
    return backing_store_;
  }

  void Transfer(ArrayBufferContents& other);
  void ShareWith(ArrayBufferContents& other);

 private:
  std::shared_ptr<v8::BackingStore> backing_store_;
};

}  // namespace nica


#endif  // CORE_TYPED_ARRAY_ARRAY_BUFFER_CONTENTS_H

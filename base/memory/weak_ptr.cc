// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/memory/weak_ptr.h"

namespace base {
namespace internal {

WeakReference::Flag::Flag() {}

void WeakReference::Flag::Invalidate() {
  invalidated_.Set();
}

bool WeakReference::Flag::IsValid() const {
  return !invalidated_.IsSet();
}

bool WeakReference::Flag::MaybeValid() const {
  return !invalidated_.IsSet();
}

WeakReference::Flag::~Flag() = default;

WeakReference::WeakReference() = default;

WeakReference::WeakReference(const scoped_refptr<Flag>& flag) : flag_(flag) {}

WeakReference::~WeakReference() = default;

WeakReference::WeakReference(WeakReference&& other) noexcept = default;

WeakReference::WeakReference(const WeakReference& other) = default;

bool WeakReference::IsValid() const {
  return flag_ && flag_->IsValid();
}

bool WeakReference::MaybeValid() const {
  return flag_ && flag_->MaybeValid();
}

WeakReferenceOwner::WeakReferenceOwner()
    : flag_(MakeRefCounted<WeakReference::Flag>()) {}

WeakReferenceOwner::~WeakReferenceOwner() {
  flag_->Invalidate();
}

WeakReference WeakReferenceOwner::GetRef() const {
  return WeakReference(flag_);
}

void WeakReferenceOwner::Invalidate() {
  flag_->Invalidate();
  flag_ = MakeRefCounted<WeakReference::Flag>();
}

WeakPtrBase::WeakPtrBase() : ptr_(0) {}

WeakPtrBase::~WeakPtrBase() = default;

WeakPtrBase::WeakPtrBase(const WeakReference& ref, uintptr_t ptr)
    : ref_(ref), ptr_(ptr) {
  DCHECK(ptr_);
}

WeakPtrFactoryBase::WeakPtrFactoryBase(uintptr_t ptr) : ptr_(ptr) {
  DCHECK(ptr_);
}

WeakPtrFactoryBase::~WeakPtrFactoryBase() {
  ptr_ = 0;
}

}  // namespace internal
}  // namespace base

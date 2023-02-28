// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MEMORY_REF_COUNTED_H_
#define BASE_MEMORY_REF_COUNTED_H_

#include <stddef.h>

#include <utility>

#include "base/atomic_ref_count.h"
#include "base/base_export.h"
#include "base/check_op.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_refptr.h"
// #include "base/gtest_prod_util.h"
// #include "base/memory/scoped_refptr.h"
// #include "base/sequence_checker.h"
// #include "base/template_util.h"
// #include "base/threading/thread_collision_warner.h"
#include "build/build_config.h"

namespace base {
namespace subtle {

class BASE_EXPORT RefCountedThreadSafeBase {
 public:
  RefCountedThreadSafeBase(const RefCountedThreadSafeBase&) = delete;
  RefCountedThreadSafeBase& operator=(const RefCountedThreadSafeBase&) = delete;

  bool HasOneRef() const;
  bool HasAtLeastOneRef() const;

 protected:
  explicit constexpr RefCountedThreadSafeBase(StartRefCountFromZeroTag) {}
  explicit constexpr RefCountedThreadSafeBase(StartRefCountFromOneTag)
      : ref_count_(1) {}

  ~RefCountedThreadSafeBase() = default;

// Release and AddRef are suitable for inlining on X86 because they generate
// very small code sequences. On other platforms (ARM), it causes a size
// regression and is probably not worth it.
#if defined(ARCH_CPU_X86_FAMILY)
  // Returns true if the object should self-delete.
  bool Release() const { return ReleaseImpl(); }
  void AddRef() const { AddRefImpl(); }
  void AddRefWithCheck() const { AddRefWithCheckImpl(); }
#else
  // Returns true if the object should self-delete.
  bool Release() const;
  void AddRef() const;
  void AddRefWithCheck() const;
#endif

 private:
  template <typename U>
  friend scoped_refptr<U> base::AdoptRef(U*);

  void Adopted() const {
  }

  ALWAYS_INLINE void AddRefImpl() const {
    ref_count_.Increment();
  }

  ALWAYS_INLINE void AddRefWithCheckImpl() const {
    CHECK_GT(ref_count_.Increment(), 0);
  }

  ALWAYS_INLINE bool ReleaseImpl() const {
    if (!ref_count_.Decrement()) {
      return true;
    }
    return false;
  }

  mutable AtomicRefCount ref_count_{0};
};

}  // namespace subtle

#define REQUIRE_ADOPTION_FOR_REFCOUNTED_TYPE()             \
  static constexpr ::base::subtle::StartRefCountFromOneTag \
      kRefCountPreference = ::base::subtle::kStartRefCountFromOneTag


// Forward declaration.
template <class T, typename Traits> class RefCountedThreadSafe;

// Default traits for RefCountedThreadSafe<T>.  Deletes the object when its ref
// count reaches 0.  Overload to delete it on a different thread etc.
template<typename T>
struct DefaultRefCountedThreadSafeTraits {
  static void Destruct(const T* x) {
    // Delete through RefCountedThreadSafe to make child classes only need to be
    // friend with RefCountedThreadSafe instead of this struct, which is an
    // implementation detail.
    RefCountedThreadSafe<T,
                         DefaultRefCountedThreadSafeTraits>::DeleteInternal(x);
  }
};

//
// A thread-safe variant of RefCounted<T>
//
//   class MyFoo : public base::RefCountedThreadSafe<MyFoo> {
//    ...
//   };
//
// If you're using the default trait, then you should add compile time
// asserts that no one else is deleting your object.  i.e.
//    private:
//     friend class base::RefCountedThreadSafe<MyFoo>;
//     ~MyFoo();
//
// We can use REQUIRE_ADOPTION_FOR_REFCOUNTED_TYPE() with RefCountedThreadSafe
// too. See the comment above the RefCounted definition for details.
template <class T, typename Traits = DefaultRefCountedThreadSafeTraits<T> >
class RefCountedThreadSafe : public subtle::RefCountedThreadSafeBase {
 public:
  static constexpr subtle::StartRefCountFromZeroTag kRefCountPreference =
      subtle::kStartRefCountFromZeroTag;

  explicit RefCountedThreadSafe()
      : subtle::RefCountedThreadSafeBase(T::kRefCountPreference) {}

  RefCountedThreadSafe(const RefCountedThreadSafe&) = delete;
  RefCountedThreadSafe& operator=(const RefCountedThreadSafe&) = delete;

  void AddRef() const { AddRefImpl(T::kRefCountPreference); }

  void Release() const {
    if (subtle::RefCountedThreadSafeBase::Release()) {
      ANALYZER_SKIP_THIS_PATH();
      Traits::Destruct(static_cast<const T*>(this));
    }
  }

 protected:
  ~RefCountedThreadSafe() = default;

 private:
  friend struct DefaultRefCountedThreadSafeTraits<T>;
  template <typename U>
  static void DeleteInternal(const U* x) {
    delete x;
  }

  void AddRefImpl(subtle::StartRefCountFromZeroTag) const {
    subtle::RefCountedThreadSafeBase::AddRef();
  }

  void AddRefImpl(subtle::StartRefCountFromOneTag) const {
    subtle::RefCountedThreadSafeBase::AddRefWithCheck();
  }
};

}  // namespace base

#endif  // BASE_MEMORY_REF_COUNTED_H_

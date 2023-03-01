// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_BIND_H_
#define BASE_BIND_H_

#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

#include "base/bind_internal.h"
#include "base/compiler_specific.h"
// #include "base/template_util.h"
#include "build/build_config.h"


// -----------------------------------------------------------------------------
// Usage documentation
// -----------------------------------------------------------------------------
//
// Overview:
// base::BindOnce() and base::BindRepeating() are helpers for creating
// base::OnceCallback and base::RepeatingCallback objects respectively.
//
// For a runnable object of n-arity, the base::Bind*() family allows partial
// application of the first m arguments. The remaining n - m arguments must be
// passed when invoking the callback with Run().
//
//   // The first argument is bound at callback creation; the remaining
//   // two must be passed when calling Run() on the callback object.
//   base::OnceCallback<long(int, long)> cb = base::BindOnce(
//       [](short x, int y, long z) { return x * y * z; }, 42);
//
// When binding to a method, the receiver object must also be specified at
// callback creation time. When Run() is invoked, the method will be invoked on
// the specified receiver object.
//
//   class C : public base::RefCounted<C> { void F(); };
//   auto instance = base::MakeRefCounted<C>();
//   auto cb = base::BindOnce(&C::F, instance);
//   std::move(cb).Run();  // Identical to instance->F()
//
// See //docs/callback.md for the full documentation.
//
// -----------------------------------------------------------------------------
// Implementation notes
// -----------------------------------------------------------------------------
//
// If you're reading the implementation, before proceeding further, you should
// read the top comment of base/bind_internal.h for a definition of common
// terms and concepts.

namespace base {

// Bind as OnceCallback.
template <typename Functor, typename... Args>
inline OnceCallback<internal::MakeUnboundRunType<Functor, Args...>> BindOnce(
    Functor&& functor,
    Args&&... args) {
  static_assert(!internal::IsOnceCallback<std::decay_t<Functor>>() ||
                    (std::is_rvalue_reference<Functor&&>() &&
                     !std::is_const<std::remove_reference_t<Functor>>()),
                "BindOnce requires non-const rvalue for OnceCallback binding."
                " I.e.: base::BindOnce(std::move(callback)).");

  return internal::BindImpl<OnceCallback>(std::forward<Functor>(functor),
                                          std::forward<Args>(args)...);
}

// Bind as RepeatingCallback.
template <typename Functor, typename... Args>
inline RepeatingCallback<internal::MakeUnboundRunType<Functor, Args...>>
BindRepeating(Functor&& functor, Args&&... args) {
  static_assert(
      !internal::IsOnceCallback<std::decay_t<Functor>>(),
      "BindRepeating cannot bind OnceCallback. Use BindOnce with std::move().");

  return internal::BindImpl<RepeatingCallback>(std::forward<Functor>(functor),
                                               std::forward<Args>(args)...);
}

// Overloads to allow nicer compile errors when attempting to pass the address
// an overloaded function to `BindOnce()` or `BindRepeating()`. Otherwise, clang
// provides only the error message "no matching function [...] candidate
// template ignored: couldn't infer template argument 'Functor'", with no
// reference to the fact that `&` is being used on an overloaded function.
//
// These overloads to provide better error messages will never be selected
// unless template type deduction fails because of how overload resolution
// works; per [over.ics.rank/2.2]:
//
//   When comparing the basic forms of implicit conversion sequences (as defined
//   in [over.best.ics])
//   - a standard conversion sequence is a better conversion sequence than a
//     user-defined conversion sequence or an ellipsis conversion sequence, and
//   - a user-defined conversion sequence is a better conversion sequence than
//     an ellipsis conversion sequence.
//
// So these overloads will only be selected as a last resort iff template type
// deduction fails.
//
// These overloads also intentionally do not return `void`, as this prevents
// clang from emitting spurious errors such as "variable has incomplete type
// 'void'" when assigning the result of `BindOnce()`/`BindRepeating()` to a
// variable with type `auto` or `decltype(auto)`.
struct BindFailedCheckPreviousErrors {};
BindFailedCheckPreviousErrors BindOnce(...);
BindFailedCheckPreviousErrors BindRepeating(...);

}  // namespace base

#endif  // BASE_BIND_H_

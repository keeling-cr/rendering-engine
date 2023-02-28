// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/synchronization/atomic_flag.h"

#include "base/check_op.h"

namespace base {

AtomicFlag::AtomicFlag() {}

AtomicFlag::~AtomicFlag() = default;

void AtomicFlag::Set() {
  flag_.store(1, std::memory_order_release);
}

}  // namespace base

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MINI_CHROMIUM_BASE_FILES_FILE_H_
#define MINI_CHROMIUM_BASE_FILES_FILE_H_

#include <stdint.h>
#include <string>

#include "build/build_config.h"

#if BUILDFLAG(IS_APPLE)
struct stat;
namespace base {
typedef struct stat stat_wrapper_t;
}
#elif BUILDFLAG(IS_POSIX)
struct stat64;
namespace base {
typedef struct stat64 stat_wrapper_t;
}
#endif

namespace base {

class File {
 public:
  File();
  File(const File&) = delete;
  File& operator=(const File&) = delete;
  ~File();

  // Destroying this object closes the file automatically.
  void Close();
#if BUILDFLAG(IS_POSIX)
  static int Fstat(int fd, stat_wrapper_t* sb);
#endif
};

} // namespace base
#endif // MINI_CHROMIUM_BASE_FILES_FILE_H_ 
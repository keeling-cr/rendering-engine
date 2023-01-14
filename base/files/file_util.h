// Copyright 2006-2008 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MINI_CHROMIUM_BASE_FILES_FILE_UTIL_H_
#define MINI_CHROMIUM_BASE_FILES_FILE_UTIL_H_

#include "build/build_config.h"
#include "base/files/file_path.h"

#if BUILDFLAG(IS_POSIX)
#include <sys/types.h>
#endif  // BUILDFLAG(IS_POSIX)

#if BUILDFLAG(IS_WIN)
#include "base/win/windows_types.h"
#elif BUILDFLAG(IS_POSIX)
#include <sys/stat.h>
#include <unistd.h>
// #include "base/file_descriptor_posix.h"
#include "base/posix/eintr_wrapper.h"
#endif

namespace base {

#if BUILDFLAG(IS_POSIX)
bool ReadFromFD(int fd, char* buffer, size_t bytes);
#endif  // BUILDFLAG(IS_POSIX)

// Reads the file at |path| into |contents| and returns true on success and
// false on error.  For security reasons, a |path| containing path traversal
// components ('..') is treated as a read error and |contents| is set to empty.
// In case of I/O error, |contents| holds the data that could be read from the
// file before the error occurred.
// |contents| may be NULL, in which case this function is useful for its side
// effect of priming the disk cache (could be used for unit tests).
bool ReadFileToString(const FilePath& path, std::string* contents);

// Reads the file at |path| into |contents| and returns true on success and
// false on error.  For security reasons, a |path| containing path traversal
// components ('..') is treated as a read error and |contents| is set to empty.
// In case of I/O error, |contents| holds the data that could be read from the
// file before the error occurred.  When the file size exceeds |max_size|, the
// function returns false with |contents| holding the file truncated to
// |max_size|.
// |contents| may be NULL, in which case this function is useful for its side
// effect of priming the disk cache (could be used for unit tests).
bool ReadFileToStringWithMaxSize(const FilePath& path,
                                std::string* contents,
                                size_t max_size);

// Wrapper for fopen-like calls. Returns non-NULL FILE* on success. The
// underlying file descriptor (POSIX) or handle (Windows) is unconditionally
// configured to not be propagated to child processes.
FILE* OpenFile(const FilePath& filename, const char* mode);


// Sets the given |fd| to close-on-exec mode.
// Returns true if it was able to set it in the close-on-exec mode, otherwise
// false.
bool SetCloseOnExec(int fd);

// As ReadFileToStringWithMaxSize, but reading from an open stream after seeking
// to its start (if supported by the stream).
bool ReadStreamToStringWithMaxSize(FILE* stream,
                                    size_t max_size,
                                    std::string* contents);

}  // namespace base


#endif  // MINI_CHROMIUM_BASE_FILES_FILE_UTIL_H_

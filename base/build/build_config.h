// Copyright 2008 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MINI_CHROMIUM_BUILD_BUILD_CONFIG_H_
#define MINI_CHROMIUM_BUILD_BUILD_CONFIG_H_

#include "build/buildflag.h"

// A set of macros to use for platform detection.
#if defined(__native_client__)
// __native_client__ must be first, so that other OS_ defines are not set.
#define OS_NACL 1
// OS_NACL comes in two sandboxing technology flavors, SFI or Non-SFI.
// PNaCl toolchain defines __native_client_nonsfi__ macro in Non-SFI build
// mode, while it does not in SFI build mode.
#if defined(__native_client_nonsfi__)
#define OS_NACL_NONSFI
#else
#define OS_NACL_SFI
#endif
#elif defined(ANDROID)
#define OS_ANDROID 1
#elif defined(__APPLE__)
// Only include TargetConditionals after testing ANDROID as some Android builds
// on the Mac have this header available and it's not needed unless the target
// is really an Apple platform.
#include <TargetConditionals.h>
#if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#define OS_IOS 1
#else
#define OS_MAC 1
#endif  // defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#elif defined(__linux__)
#if !defined(OS_CHROMEOS)
// Do not define OS_LINUX on Chrome OS build.
// The OS_CHROMEOS macro is defined in GN.
#define OS_LINUX 1
#endif  // !defined(OS_CHROMEOS)
// Include a system header to pull in features.h for glibc/uclibc macros.
#include <unistd.h>
#if defined(__GLIBC__) && !defined(__UCLIBC__)
// We really are using glibc, not uClibc pretending to be glibc.
#define LIBC_GLIBC 1
#endif
#elif defined(_WIN32)
#define OS_WIN 1
#elif defined(__Fuchsia__)
#define OS_FUCHSIA 1
#elif defined(__FreeBSD__)
#define OS_FREEBSD 1
#elif defined(__NetBSD__)
#define OS_NETBSD 1
#elif defined(__OpenBSD__)
#define OS_OPENBSD 1
#elif defined(__sun)
#define OS_SOLARIS 1
#elif defined(__QNXNTO__)
#define OS_QNX 1
#elif defined(_AIX)
#define OS_AIX 1
#elif defined(__asmjs__) || defined(__wasm__)
#define OS_ASMJS 1
#else
#error Please add support for your platform in build/build_config.h
#endif
// NOTE: Adding a new port? Please follow
// https://chromium.googlesource.com/chromium/src/+/main/docs/new_port_policy.md

#if defined(OS_MAC) || defined(OS_IOS)
#define OS_APPLE 1
#endif

// For access to standard BSD features, use OS_BSD instead of a
// more specific macro.
#if defined(OS_FREEBSD) || defined(OS_NETBSD) || defined(OS_OPENBSD)
#define OS_BSD 1
#endif

// For access to standard POSIXish features, use OS_POSIX instead of a
// more specific macro.
#if defined(OS_AIX) || defined(OS_ANDROID) || defined(OS_ASMJS) ||  \
    defined(OS_FREEBSD) || defined(OS_IOS) || defined(OS_LINUX) ||  \
    defined(OS_CHROMEOS) || defined(OS_MAC) || defined(OS_NACL) ||  \
    defined(OS_NETBSD) || defined(OS_OPENBSD) || defined(OS_QNX) || \
    defined(OS_SOLARIS)
#define OS_POSIX 1
#endif

#if defined(__APPLE__)
#include <TargetConditionals.h>
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_APPLE() 1
#if defined(TARGET_OS_OSX) && TARGET_OS_OSX
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_MAC() 1
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_IOS() 0
#elif defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_MAC() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_IOS() 1
#endif  // TARGET_OS_*
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_ANDROID() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_LINUX() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_CHROMEOS() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_WIN() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_FUCHSIA() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_POSIX() 1
#elif defined(__ANDROID__)
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_APPLE() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_MAC() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_IOS() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_ANDROID() 1
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_LINUX() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_CHROMEOS() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_WIN() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_FUCHSIA() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_POSIX() 1
#elif defined(__linux__)
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_APPLE() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_MAC() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_IOS() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_ANDROID() 0
#if !defined(OS_CHROMEOS)
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_LINUX() 1
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_CHROMEOS() 0
#else
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_LINUX() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_CHROMEOS() 1
#endif
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_WIN() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_FUCHSIA() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_POSIX() 1
#elif defined(_WIN32)
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_APPLE() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_MAC() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_IOS() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_ANDROID() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_LINUX() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_CHROMEOS() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_WIN() 1
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_FUCHSIA() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_POSIX() 0
#elif defined(__Fuchsia__)
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_APPLE() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_MAC() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_IOS() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_ANDROID() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_LINUX() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_CHROMEOS() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_WIN() 0
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_FUCHSIA() 1
#define MINI_CHROMIUM_INTERNAL_BUILDFLAG_VALUE_IS_POSIX() 1
#else
#error Please add support for your platform in build/build_config.h
#endif

// Compiler detection.
#if defined(__GNUC__)
#define COMPILER_GCC 1
#elif defined(_MSC_VER)
#define COMPILER_MSVC 1
#else
#error Please add support for your compiler in build/build_config.h
#endif

#if defined(_M_X64) || defined(__x86_64__)
#define ARCH_CPU_X86_FAMILY 1
#define ARCH_CPU_X86_64 1
#define ARCH_CPU_64_BITS 1
#define ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(_M_IX86) || defined(__i386__)
#define ARCH_CPU_X86_FAMILY 1
#define ARCH_CPU_X86 1
#define ARCH_CPU_32_BITS 1
#define ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__ARMEL__)
#define ARCH_CPU_ARM_FAMILY 1
#define ARCH_CPU_ARMEL 1
#define ARCH_CPU_32_BITS 1
#elif defined(_M_ARM64) || defined(__aarch64__)
#define ARCH_CPU_ARM_FAMILY 1
#define ARCH_CPU_ARM64 1
#define ARCH_CPU_64_BITS 1
#if defined(_M_ARM64)
#define ARCH_CPU_LITTLE_ENDIAN 1
#endif
#elif defined(__MIPSEL__)
#define ARCH_CPU_MIPS_FAMILY 1
#if !defined(__LP64__)
#define ARCH_CPU_MIPSEL 1
#define ARCH_CPU_32_BITS 1
#else
#define ARCH_CPU_MIPS64EL 1
#define ARCH_CPU_64_BITS 1
#endif
#else
#error Please add support for your architecture in build/build_config.h
#endif

#if !defined(ARCH_CPU_LITTLE_ENDIAN) && !defined(ARCH_CPU_BIG_ENDIAN)
#if defined(__LITTLE_ENDIAN__) || \
    (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__BIG_ENDIAN__) || \
    (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define ARCH_CPU_BIG_ENDIAN 1
#else
#error Please add support for your architecture in build/build_config.h
#endif
#endif

#if BUILDFLAG(IS_POSIX) && defined(COMPILER_GCC) && defined(__WCHAR_MAX__) && \
    (__WCHAR_MAX__ == 0x7fffffff || __WCHAR_MAX__ == 0xffffffff)
#define WCHAR_T_IS_UTF32
#elif BUILDFLAG(IS_WIN)
#define WCHAR_T_IS_UTF16
#else
#error Please add support for your compiler in build/build_config.h
#endif

#endif  // MINI_CHROMIUM_BUILD_BUILD_CONFIG_H_

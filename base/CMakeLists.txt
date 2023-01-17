set(BASE_SOURCE 
    files/file_path.cc
    files/file_util.cc
    files/file.cc
    files/scoped_file.cc
    logging.cc
    strings/string_number_conversions.cc
    strings/string_util.cc
    strings/stringprintf.cc
    strings/utf_string_conversions.cc
    strings/utf_string_conversion_utils.cc
    third_party/icu/icu_utf.cc
    )

if (APPLE)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    find_library(COREGRAPHICS_LIBRARY CoreGraphics)
    find_library(CORETEXT_LIBRARY CoreText)
    find_library(FOUNDARION_LIBRARY Foundation)
    find_library(SECURITY_LIBRARY Security)

    list(APPEND BASE_LIBRARIES
        ${CARBON_LIBRARY}
        ${COCOA_LIBRARY}
        ${COREFOUNDATION_LIBRARY}
    )

    list(APPEND BASE_SOURCE
        files/file_util_posix.cc
        files/file_posix.cc
        mac/close_nocancel.cc
        mac/foundation_util.mm
        mac/mach_logging.cc
        mac/scoped_mach_port.cc
        mac/scoped_mach_vm.cc
        mac/scoped_nsautorelease_pool.mm
        strings/sys_string_conversions_mac.mm
    )
endif ()

if (APPLE)
    list(APPEND BASE_SOURCE
        posix/safe_strerror.cc
    )
endif ()

if (WIN)
    list(APPEND BASE_LIBRARIES
        scoped_clear_last_error_win.cc
    )
endif ()

add_library(libbase ${BASE_SOURCE})

target_link_libraries(libbase PUBLIC ${BASE_LIBRARIES})
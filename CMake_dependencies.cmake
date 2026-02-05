if(NOT DEFINED CPM_USE_LOCAL_PACKAGES)
    set(CPM_USE_LOCAL_PACKAGES ${otfccxx_USE_LOCAL_PACKAGES} CACHE BOOL "CPM will try to find packages locally first" FORCE)
endif()
if(NOT DEFINED CPM_LOCAL_PACKAGES_ONLY)
    set(CPM_LOCAL_PACKAGES_ONLY ${otfccxx_USE_LOCAL_PACKAGES_ONLY} CACHE BOOL
        "CPM will not be forbidden from downloading packages. Will have to use local packages." FORCE)
endif()

include(cmake/CPM.cmake)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

CPMAddPackage(
    NAME harfbuzz
    URL https://github.com/harfbuzz/harfbuzz/releases/download/12.3.0/harfbuzz-12.3.0.tar.xz
    URL_HASH SHA256=8660ebd3c27d9407fc8433b5d172bafba5f0317cb0bb4339f28e5370c93d42b7
    EXCLUDE_FROM_ALL TRUE
    OPTIONS "BUILD_SHARED_LIBS ON" "HB_BUILD_UTILS OFF"
)
CPMAddPackage(
    URI "gh:InCom-0/otfcc-lib_cmake#master"
    OPTIONS "BUILD_SHARED_LIBS OFF"
)
CPMAddPackage(
    URI "gh:InCom-0/fmem#master"
    OPTIONS "FMEM_STATIC ON" "ALLOW_OPENMEMSTREAM OFF"
)
CPMAddPackage(
    URI "gh:InCom-0/woff2#otfccxx"
    OPTIONS "NOISY_LOGGING OFF" "BUILD_SHARED_LIBS OFF"
    NAME WOFF2
)
CPMAddPackage(
    URI "gh:InCom-0/base64#master"
    OPTIONS "BASE64_ENABLE_TESTING OFF"
    NAME base64tl
)

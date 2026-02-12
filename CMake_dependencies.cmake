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

find_package(WOFF2)

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


message(STATUS "---- HarfBuzz / WOFF2 diagnostics ----")

# HarfBuzz
message(STATUS "HarfBuzz_DIR='${HarfBuzz_DIR}'")
message(STATUS "HarfBuzz_FOUND='${HarfBuzz_FOUND}'")
message(STATUS "HarfBuzz_INCLUDE_DIRS='${HarfBuzz_INCLUDE_DIRS}'")
message(STATUS "HarfBuzz_LIBRARIES='${HarfBuzz_LIBRARIES}'")
if(TARGET harfbuzz::harfbuzz)
    message(STATUS "Target harfbuzz::harfbuzz = FOUND")
elseif(TARGET HarfBuzz::HarfBuzz)
    message(STATUS "Target HarfBuzz::HarfBuzz = FOUND")
else()
    message(STATUS "HarfBuzz target = NOT FOUND")
endif()

# WOFF2
message(STATUS "WOFF2_DIR='${WOFF2_DIR}'")
message(STATUS "WOFF2_FOUND='${WOFF2_FOUND}'")
message(STATUS "WOFF2_INCLUDE_DIRS='${WOFF2_INCLUDE_DIRS}'")
message(STATUS "WOFF2_LIBRARIES='${WOFF2_LIBRARIES}'")
if(TARGET WOFF2::woff2)
    message(STATUS "Target WOFF2::woff2 = FOUND")
elseif(TARGET woff2::woff2)
    message(STATUS "Target woff2::woff2 = FOUND")
else()
    message(STATUS "WOFF2 target = NOT FOUND")
endif()

message(STATUS "--------------------------------------")

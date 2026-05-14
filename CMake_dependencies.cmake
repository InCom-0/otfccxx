include(cmake/CPM_0.42.1.cmake)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/incom/modules")

CPMAddPackage(
    NAME harfbuzz
    URL https://github.com/harfbuzz/harfbuzz/releases/download/14.2.0/harfbuzz-14.2.0.tar.xz
    URL_HASH SHA256=94017020f96d025bb66ae91574e4cf334bcad23e8175a8a40565b3721bc2eaff
    OPTIONS "BUILD_SHARED_LIBS ON" "HB_BUILD_UTILS OFF"
    EXCLUDE_FROM_ALL TRUE
)
if(harfbuzz_ADDED)
    add_library(harfbuzz::harfbuzz ALIAS harfbuzz)
    add_library(harfbuzz::subset ALIAS harfbuzz-subset)
endif()

CPMAddPackage(
    URI "gh:InCom-0/woff2#otfccxx"
    OPTIONS "NOISY_LOGGING OFF" "BUILD_SHARED_LIBS OFF"
    NAME WOFF2
    FIND_PACKAGE_ARGUMENTS "COMPONENTS woff2dec woff2enc"
)


########################################################
### Vendored dependencies ###
########################################################
set(CPM_otfcc_cmake_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/otfcc_cmake)
CPMAddPackage(
    URI "gh:InCom-0/otfcc_cmake#master"
    OPTIONS "BUILD_SHARED_LIBS OFF"
)

set(CPM_base64tl_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/base64)
CPMAddPackage(
    URI "gh:InCom-0/base64#master"
    OPTIONS "BASE64_ENABLE_TESTING OFF"
    NAME base64tl
)

set(CPM_fmem_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/fmem)
CPMAddPackage(
    URI "gh:InCom-0/fmem#master"
    OPTIONS "FMEM_STATIC ON" "ALLOW_OPENMEMSTREAM OFF"
)
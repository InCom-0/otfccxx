include(cmake/CPM.cmake)
set(CPM_USE_LOCAL_PACKAGES_ORIG ${CPM_USE_LOCAL_PACKAGES})
set(CPM_USE_LOCAL_PACKAGES ON)

CPMAddPackage("gh:InCom-0/fmem#master")
CPMAddPackage("gh:InCom-0/otfcc-lib_cmake#master")

set(HB_BUILD_UTILS OFF)
# CPMAddPackage(
#   NAME     harfbuzz
#   URL      https://github.com/harfbuzz/harfbuzz/releases/download/12.2.0/harfbuzz-12.2.0.tar.xz
#   URL_HASH SHA256=ecb603aa426a8b24665718667bda64a84c1504db7454ee4cadbd362eea64e545
#   EXCLUDE_FROM_ALL TRUE
# )

set(CPM_USE_LOCAL_PACKAGES OFF)
CPMAddPackage("gh:InCom-0/harfbuzz#symbolDedup")
CPMAddPackage(
    URI "gh:InCom-0/woff2#otfccxx"
    OPTIONS "NOISY_LOGGING OFF"
)

set(CPM_USE_LOCAL_PACKAGES ${CPM_USE_LOCAL_PACKAGES_ORIG})

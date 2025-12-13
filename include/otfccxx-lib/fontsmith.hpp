#pragma once

#include <cstddef>
#include <cstdint>
#include <expected>
#include <filesystem>

#include <span>


#include <hb-subset.hh>
// #include <nlohmann/json.hpp>
#include <otfcc/font.h>
#include <otfcc/sfnt.h>


namespace fontsmith {

enum class err : size_t {
    uknownError = 1,
    hb_blob_t_createFailure,
    hb_face_t_createFailure,
    execute_someRequestedGlyphsAreMissing,
    subsetInput_failedToCreate,
    hb_subset_executeFailure,
    make_subset_noIntersectingGlyphs,
    unexpectedNullptr,
};
namespace detail {
struct _hb_face_uptr_deleter {
    void operator()(hb_face_t *f) const noexcept {
        if (f) { hb_face_destroy(f); }
    }
};
struct _hb_blob_uptr_deleter {
    void operator()(hb_blob_t *b) const noexcept {
        if (b) { hb_blob_destroy(b); }
    }
};
struct _hb_set_uptr_deleter {
    void operator()(hb_set_t *s) const noexcept {
        if (s) { hb_set_destroy(s); }
    }
};
struct _hb_subset_input_uptr_deleter {
    void operator()(hb_subset_input_t *s) const noexcept {
        if (s) { hb_subset_input_destroy(s); }
    }
};
} // namespace detail

using hb_face_uptr         = std::unique_ptr<hb_face_t, detail::_hb_face_uptr_deleter>;
using hb_blob_uptr         = std::unique_ptr<hb_blob_t, detail::_hb_blob_uptr_deleter>;
using hb_set_uptr          = std::unique_ptr<hb_set_t, detail::_hb_set_uptr_deleter>;
using hb_subset_input_uptr = std::unique_ptr<hb_subset_input_t, detail::_hb_subset_input_uptr_deleter>;

using font_raw = std::vector<std::byte>;


std::expected<bool, std::filesystem::file_type> write_bytesToFile(std::filesystem::path const &p,
                                                                  std::span<const std::byte>   bytes);

class Subsetter {
private:
    class Impl;

public:
    Subsetter();                                 // defined in the implementation file

    Subsetter(int);                              // defined in the implementation file
    ~Subsetter();                                // defined in the implementation file, where impl is a complete type
    Subsetter(Subsetter &&) noexcept;            // defined in the implementation file
    Subsetter(const Subsetter &) = delete;
    Subsetter &operator=(Subsetter &&) noexcept; // defined in the implementation file
    Subsetter &operator=(const Subsetter &) = delete;


    Subsetter &add_ff_toSubset(std::span<const char> buf, unsigned int const faceIndex = 0u);
    Subsetter &add_ff_categoryBackup(std::span<const char> buf, unsigned int const faceIndex = 0u);
    Subsetter &add_ff_lastResort(std::span<const char> buf, unsigned int const faceIndex = 0u);

    Subsetter &add_ff_toSubset(std::filesystem::path const &pth, unsigned int const faceIndex = 0u);
    Subsetter &add_ff_categoryBackup(std::filesystem::path const &pth, unsigned int const faceIndex = 0u);
    Subsetter &add_ff_lastResort(std::filesystem::path const &pth, unsigned int const faceIndex = 0u);

    // Subsetter &add_ff_toSubset(hb_face_t *ptr, unsigned int const faceIndex = 0u);
    // Subsetter &add_ff_categoryBackup(hb_face_t *ptr, unsigned int const faceIndex = 0u);
    // Subsetter &add_ff_lastResort(hb_face_t *ptr, unsigned int const faceIndex = 0u);

    Subsetter &add_toKeep_CP(uint32_t cp);
    Subsetter &add_toKeep_CPs(std::span<const uint32_t> cps);

    // 1) execute() - Get 'waterfall of font faces'
    // 2) execute_bestEffort() - Get 'waterfall of font faces' + set a unicode points that weren't found in any font
    std::expected<std::vector<font_raw>, err>                         execute();
    std::expected<std::pair<std::vector<font_raw>, std::vector<uint32_t>>, err> execute_bestEffort();


    bool is_inError();
    err  get_error();


private:
    std::unique_ptr<Impl> pimpl;
};


class CompatMaker {
public:
private:
};

} // namespace fontsmith
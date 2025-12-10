#pragma once


#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <expected>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include <hb-subset.hh>
#include <nlohmann/json.hpp>
#include <otfcc/font.h>
#include <otfcc/sfnt.h>


namespace fontsmith {

class Subsetter {
    using faceUPtr = std::unique_ptr<hb_face_t, decltype([](hb_face_t *f) noexcept {
                                         if (f) { hb_face_destroy(f); }
                                     })>;


public:
    Subsetter &add_ff_toSubset(std::span<const char> buf);
    Subsetter &add_ff_categoryBackup(std::span<const char> buf);
    Subsetter &add_ff_lastResort(std::span<const char> buf);

private:
    std::vector<faceUPtr> ffs_toSubset;

    std::vector<faceUPtr> ffs_categoryBackup;

    std::vector<faceUPtr> ffs_lastResort;
};

} // namespace fontsmith
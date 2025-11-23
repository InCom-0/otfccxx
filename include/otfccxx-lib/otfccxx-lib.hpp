#pragma once

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <memory>
#include <optional>
#include <otfcc/font.h>
#include <otfcc/primitives.h>
#include <otfcc/table/cmap.h>
#include <otfcc/table/glyf.h>
#include <otfcc/vf/vq.h>
#include <span>
#include <utility>
#include <vector>

namespace otfccxx {
namespace detail {

// Caryll vector. Namespace providing modernish C++ interface to caryll vector based types
namespace cv {
inline auto get_rngInterface(auto &caryll_vector_t) {
    return std::span<decltype(caryll_vector_t.items)>(&caryll_vector_t.items, caryll_vector_t.length);
};


} // namespace cv

inline void Transform(glyf_Glyph &glyph, double a, double b, double c, double d, double dx, double dy) {

    auto adj_VQstill = [](VQ &VQ_out, auto const &func) {
        func(VQ_out.kernel);
        for (size_t j = 0; j < VQ_out.shift.length; j++) {
            if (VQ_out.shift.items[j].type == VQ_STILL) { func(VQ_out.shift.items[j].val.still); }
        }
    };
    auto multStill = [&](pos_t &out_still) { out_still = std::round(a * out_still); };
    adj_VQstill(glyph.advanceWidth, multStill);
    adj_VQstill(glyph.advanceHeight, multStill);

    auto transform_porr = [&](auto &out_porr) {
        auto &one_gp_ref = (*out_porr);
        VQ    xCpy       = iVQ.dup(one_gp_ref.x);

        one_gp_ref.x.kernel = static_cast<int>(a * xCpy.kernel + b * one_gp_ref.y.kernel + dx);
        for (size_t j = 0; j < one_gp_ref.x.shift.length; j++) {
            if (one_gp_ref.x.shift.items[j].type == VQ_STILL) {
                one_gp_ref.x.shift.items[j].val.still = static_cast<int>(
                    a * xCpy.shift.items[j].val.still + b * one_gp_ref.y.shift.items[j].val.still + dx);
            }
        }

        one_gp_ref.y.kernel = static_cast<int>(c * xCpy.kernel + d * one_gp_ref.y.kernel + dy);
        for (size_t j = 0; j < one_gp_ref.y.shift.length; j++) {
            if (one_gp_ref.y.shift.items[j].type == VQ_STILL) {
                one_gp_ref.y.shift.items[j].val.still = static_cast<int>(
                    c * xCpy.shift.items[j].val.still + d * one_gp_ref.y.shift.items[j].val.still + dy);
            }
        }
    };

    for (auto &one_cont : cv::get_rngInterface(glyph.contours)) {
        for (auto &one_gp : cv::get_rngInterface((*one_cont))) { transform_porr(one_gp); }
    }
    for (auto &one_refer : cv::get_rngInterface(glyph.references)) { transform_porr(one_refer); }
}
} // namespace detail


class Font {
private:
public:
    explicit Font() : handle_{otfcc_iFont.create()} {
        // if (! handle_) { throw std::runtime_error("Failed to create CThing"); }
    }
    explicit Font(otfcc_SplineFontContainer &sfnt, uint32_t index, otfcc_Options const &options)
        : handle_(Ctor_(sfnt, index, options)) {}

    // Non-copyable, but movable
    Font(Font &&) noexcept            = default;
    Font &operator=(Font &&) noexcept = default;

    Font copy_deep() const {
        Font res = Font();
        if (handle_) {
            otfcc_Font *CpyPtr;
            otfcc_iFont.copy(CpyPtr, handle_.get());
            res.handle_.reset(CpyPtr);
        }
        return res;
    }

    std::optional<int> filter_cmap(auto const &&filterFunc) {
        if (not handle_->cmap) { return std::nullopt; }
        auto &cmapRef = *(handle_->cmap);

        std::vector<int> ids_toUnmap;
        for (cmap_Entry const *item = cmapRef.unicodes; item != nullptr; item = (cmap_Entry *)item->hh.next) {
            cmap_Entry const &ref = *item;
            if (not filterFunc(ref)) { ids_toUnmap.push_back(ref.unicode); cmapRef.unicodes->glyph }
        }




        for (auto const toUnmap : ids_toUnmap) { table_iCmap.unmap(handle_->cmap, toUnmap); }

        return std::nullopt;
    }

    std::optional<int> filter_glyphs(auto const &&filterFunc) { return std::nullopt; }


private:
    static otfcc_Font *Ctor_(otfcc_SplineFontContainer const &sfnt, uint32_t index, otfcc_Options const &options) {
        otfcc_IFontBuilder *reader = otfcc_newOTFReader();
        auto                res = reader->read((otfcc_SplineFontContainer *)(&sfnt), index, (otfcc_Options *)&options);
        reader->free(reader);
        return res;
    }

    struct Deleter {
        void operator()(otfcc_Font *ptr) const noexcept { otfcc_iFont.free(ptr); }
    };

    std::unique_ptr<otfcc_Font, Deleter> handle_;
};


class FontMerger {
public:
    static std::optional<int> merge_intoBase_inPlace(Font &out_base, std::vector<Font> &out_toMerge) {
        FontMerger fm(out_base, out_toMerge);
        if (auto errOpt = fm._merge(); errOpt.has_value()) { return errOpt; }
        else { return std::nullopt; }
        std::unreachable();
    }
    static std::expected<Font, int> merge_intoBase(Font const &out_base, std::vector<Font> const &toMerge) {
        // Make copies, because we are not doing it inPlace
        Font base_res = out_base.copy_deep();

        std::vector<Font> toMerge_cpy;
        for (auto const &mrg_font : toMerge) { toMerge_cpy.push_back(mrg_font.copy_deep()); }
        FontMerger fm(base_res, toMerge_cpy);

        if (auto errOpt = fm._merge(); errOpt.has_value()) { return std::unexpected(errOpt.value()); }
        else { return base_res; }
        std::unreachable();
    }


    static std::optional<int> merge_intoBase_inPlace(Font &out_base, std::vector<Font> &out_toMerge,
                                                     std::vector<int> const &codePointsToKeep) {
        FontMerger fm(out_base, out_toMerge);
        if (auto errOpt = fm._merge(codePointsToKeep); errOpt.has_value()) { return errOpt; }
        else { return std::nullopt; }
        std::unreachable();
    }
    static std::expected<Font, int> merge_intoBase(Font const &out_base, std::vector<Font> const &toMerge,
                                                   std::vector<int> const &codePointsToKeep) {
        // Make copies, because we are not doing it inPlace
        Font base_res = out_base.copy_deep();

        std::vector<Font> toMerge_cpy;
        for (auto const &mrg_font : toMerge) { toMerge_cpy.push_back(mrg_font.copy_deep()); }
        FontMerger fm(base_res, toMerge_cpy);

        if (auto errOpt = fm._merge(codePointsToKeep); errOpt.has_value()) { return std::unexpected(errOpt.value()); }
        else { return base_res; }
        std::unreachable();
    }

private:
    std::optional<int> _merge() { return std::optional(1); }

    std::optional<int> _merge(std::vector<int> const &codePointsToKeep) { return std::optional(1); }

    FontMerger(Font &base, std::vector<Font> &toMerge) : base{base}, others(toMerge) {}

    Font              &base;
    std::vector<Font> &others;
};


} // namespace otfccxx
#ifndef PYSTRING_SLICE_HPP
#define PYSTRING_SLICE_HPP

#include <cstddef>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

namespace pystring {

/**
 * @brief Python-style slice object representing [start:stop:step].
 */
struct Slice {
    std::optional<ptrdiff_t> start;
    std::optional<ptrdiff_t> stop;
    std::optional<ptrdiff_t> step;

    constexpr Slice() : start(std::nullopt), stop(std::nullopt), step(std::nullopt) {}
    
    constexpr Slice(std::optional<ptrdiff_t> stop_val)
        : start(std::nullopt), stop(stop_val), step(std::nullopt) {}
        
    constexpr Slice(std::optional<ptrdiff_t> start_val, std::optional<ptrdiff_t> stop_val)
        : start(start_val), stop(stop_val), step(std::nullopt) {}
        
    constexpr Slice(std::optional<ptrdiff_t> start_val, 
                    std::optional<ptrdiff_t> stop_val, 
                    std::optional<ptrdiff_t> step_val)
        : start(start_val), stop(stop_val), step(step_val) {}

    struct ComputedIndices {
        ptrdiff_t start{0};
        ptrdiff_t stop{0};
        ptrdiff_t step{1};
        size_t length{0};
    };

    /**
     * @brief Computes concrete iteration indices according to CPython's PySlice_GetIndicesEx.
     */
    constexpr ComputedIndices compute(size_t len) const {
        ptrdiff_t st = step.value_or(1);
        if (st == 0) {
            throw std::invalid_argument("slice step cannot be zero");
        }

        ptrdiff_t s = 0;
        ptrdiff_t e = 0;
        ptrdiff_t length = static_cast<ptrdiff_t>(len);

        if (st > 0) {
            s = start.value_or(0);
            e = stop.value_or(length);

            if (s < 0) s += length;
            if (s < 0) s = 0;
            if (s > length) s = length;

            if (e < 0) e += length;
            if (e < 0) e = 0;
            if (e > length) e = length;
        } else {
            s = start.value_or(length - 1);

            if (s < 0) s += length;
            if (s < -1) s = -1;
            if (s >= length) s = length - 1;

            if (stop.has_value()) {
                e = *stop;
                if (e < 0) e += length;
                if (e < -1) e = -1;
                if (e >= length) e = length - 1;
            } else {
                e = -1;
            }
        }

        size_t slice_len = 0;
        if (st > 0) {
            if (s < e) {
                slice_len = static_cast<size_t>((e - s - 1) / st + 1);
            }
        } else {
            if (s > e) {
                slice_len = static_cast<size_t>((s - e - 1) / (-st) + 1);
            }
        }

        return {s, e, st, slice_len};
    }
};

/**
 * @brief Helper factory function to create a Slice(stop).
 */
inline constexpr Slice slice(std::optional<ptrdiff_t> stop = std::nullopt) {
    return Slice(stop);
}

/**
 * @brief Helper factory function to create a Slice(start, stop, step).
 */
inline constexpr Slice slice(std::optional<ptrdiff_t> start, 
                             std::optional<ptrdiff_t> stop, 
                             std::optional<ptrdiff_t> step = std::nullopt) {
    return Slice(start, stop, step);
}

} // namespace pystring

#endif // PYSTRING_SLICE_HPP

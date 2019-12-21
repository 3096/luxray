#pragma once

#include <cstdint>
#include <utility>
#include <type_traits>

#include "util.hpp"

struct rgba4444_t {
    union {
        struct {
            uint16_t r: 4, g: 4, b: 4, a: 4;
        } __attribute__((packed));
        uint16_t rgba;
    };
    constexpr rgba4444_t(): rgba(0) { }
    constexpr rgba4444_t(uint16_t raw): rgba(raw) { }
    constexpr rgba4444_t(uint8_t r, uint8_t g, uint8_t b, uint8_t a): r(r), g(g), b(b), a(a) { }
};
ASSERT_SIZE(rgba4444_t, 2);
ASSERT_STANDARD_LAYOUT(rgba4444_t);

template<typename T, typename=void>
struct has_alpha : std::false_type { };

template<typename T>
struct has_alpha<T, decltype(std::declval<T>().a, void())> : std::true_type { };

template<typename T>
inline constexpr bool has_alpha_v = has_alpha<T>::value;

template<typename T>
struct col_underlying_type { typedef decltype(T::r) type; };

template<typename T>
using col_underlying_type_t = typename col_underlying_type<T>::type;

template<typename T>
T make_color(col_underlying_type_t<T> raw) {
    return T(raw);
}

template<typename T>
typename std::enable_if_t<!has_alpha_v<T>, T>
make_color(decltype(T::r) r, decltype(T::g) g, decltype(T::b) b) {
    return T(r, g, b);
}

template<typename T>
typename std::enable_if_t<has_alpha_v<T>, T>
make_color(decltype(T::r) r, decltype(T::g) g, decltype(T::b) b, decltype(T::a) a=-1) {
    return T(r, g, b, a);
}

template<typename T>
typename std::enable_if_t<!has_alpha_v<T>, T>
make_color_min_alpha() {
    return make_color<T>(0, 0, 0);
}

template<typename T>
typename std::enable_if_t<has_alpha_v<T>, T>
make_color_min_alpha() {
    return make_color<T>(0, 0, 0, 0);
}

template<typename T>
T make_color_max_alpha(decltype(T::r) r, decltype(T::g) g, decltype(T::b) b) {
    return make_color<T>(r, g, b);
}

template<typename T>
T make_color_max_all() {
    return make_color<T>(-1, -1, -1);
}

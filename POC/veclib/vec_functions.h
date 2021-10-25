#pragma once


#include "vec_operators.h"
#include "math_concepts.h"
#include <cmath>


namespace zeno::ztd {
inline namespace math {


template <class T1, class T2,
          class T0 = decltype(0 ? std::declval<remove_vec_t<T1>>() : std::declval<remove_vec_t<T2>>())>
    requires (concepts::has_std_min<T0>)
constexpr decltype(auto) min(T1 const &t1, T2 const &t2) {
    return vec_wise(t1, t2, [] (auto &&t1, auto &&t2) { return (T0)std::min((T0)t1, (T0)t2); });
}


template <class T1, class T2,
          class T0 = decltype(0 ? std::declval<remove_vec_t<T1>>() : std::declval<remove_vec_t<T2>>())>
    requires (concepts::has_std_max<T0>)
constexpr decltype(auto) max(T1 const &t1, T2 const &t2) {
    return vec_wise(t1, t2, [] (auto &&t1, auto &&t2) { return (T0)std::max((T0)t1, (T0)t2); });
}


}
}
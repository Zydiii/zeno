#pragma once

#include <zs/ztd/error.h>

namespace zs::ztd {

#define __ZS_ZTD_ASSERT_TST(y, ...) y
#define __ZS_ZTD_ASSERT_EXT(xs, y, ...) __VA_OPT__(": {}"), xs __VA_ARGS__
#define ZS_ZTD_ASSERT(x, ...) (([&] (auto &&__assert_val) -> decltype(auto) { \
    [[unlikely]] if (!((__assert_val) __ZS_ZTD_ASSERT_TST(__VA_ARGS__))) \
        throw zs::ztd::format_error("AssertionError: {}" __ZS_ZTD_ASSERT_EXT(#x, __VA_ARGS__)); \
    return std::forward<decltype(__assert_val)>(__assert_val); \
})((x)))

#define ZS_ZTD_ASSERT_EQ(x, y) ZS_ZTD_ASSERT((x) == (y), fmt::format("unsatisfied: {} == {}", (x), (y)))
#define ZS_ZTD_ASSERT_NE(x, y) ZS_ZTD_ASSERT((x) != (y), fmt::format("unsatisfied: {} != {}", (x), (y)))
#define ZS_ZTD_ASSERT_GE(x, y) ZS_ZTD_ASSERT((x) >= (y), fmt::format("unsatisfied: {} >= {}", (x), (y)))
#define ZS_ZTD_ASSERT_LE(x, y) ZS_ZTD_ASSERT((x) <= (y), fmt::format("unsatisfied: {} <= {}", (x), (y)))
#define ZS_ZTD_ASSERT_GT(x, y) ZS_ZTD_ASSERT((x) > (y), fmt::format("unsatisfied: {} > {}", (x), (y)))
#define ZS_ZTD_ASSERT_LT(x, y) ZS_ZTD_ASSERT((x) < (y), fmt::format("unsatisfied: {} < {}", (x), (y)))
#define ZS_ZTD_ASSERT_FALSE(x) ZS_ZTD_ASSERT((x) == false, fmt::format("unsatisfied: {} == false", (x)))
#define ZS_ZTD_ASSERT_TRUE(x) ZS_ZTD_ASSERT((x) == true, fmt::format("unsatisfied: {} == true", (x)))
#define ZS_ZTD_ASSERT_NULL(x) ZS_ZTD_ASSERT((x) == nullptr, fmt::format("unsatisfied: {} == nullptr", (void *)(x)))
#define ZS_ZTD_ASSERT_NOTNULL(x) ZS_ZTD_ASSERT((x) != nullptr, fmt::format("unsatisfied: {} != nullptr", (void *)(x)))

#include <zeno/zty/array/Array.h>
#include <zeno/ztd/algorithm.h>
#include <zeno/ztd/variant.h>
#include <zeno/math/vec.h>
#include <functional>


ZENO_NAMESPACE_BEGIN
namespace zty {


Array arraySelectOp(BoolArray const &arr1, Array const &arr2, Array const &arr3) {
    return std::visit([&] (auto const &arr2, auto const &arr3) -> Array {
        using T = std::decay_t<decltype(arr2[0] + arr3[0])>;
        std::vector<T> arr(arr1.size() && arr2.size() && arr3.size() ? std::max({arr1.size(), arr2.size(), arr3.size()}) : 0);
        #pragma omp parallel for
        for (size_t i = 0; i < arr.size(); i++) {
            arr[i] = arr1[std::min(i, arr1.size() - 1)] ? static_cast<T>(arr2[std::min(i, arr2.size() - 1)])
                : static_cast<T>(arr3[std::min(i, arr3.size() - 1)]);
        }
        return arr;
    }, arr2.get(), arr3.get());
}


Array arraySelectOp(BoolArray const &arr1, Array const &arr2) {
    return std::visit([&] (auto const &arr2) -> Array {
        std::vector<std::decay_t<decltype(arr2[0])>> arr;
        for (size_t i = 0; i < arr1.size(); i++) {
            if (bool{arr1[i]})
                arr.push_back(arr2[i]);
        }
        return arr;
    }, arr2.get());
}


}
ZENO_NAMESPACE_END
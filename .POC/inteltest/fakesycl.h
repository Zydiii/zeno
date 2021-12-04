#pragma once

#include <zeno/common.h>
#include <array>
#include <vector>

#define ZENO_FAKESYCL 1

ZENO_NAMESPACE_BEGIN
inline namespace fakesycl {
namespace sycl {

using cl_int = int;
using cl_float = float;

template <size_t N>
struct id : std::array<size_t, N> {
    using std::array<size_t, N>::array;

    constexpr explicit(N != 1) id(size_t i)
        : std::array<size_t, N>({i}) {
    }

    constexpr explicit(N != 1) operator size_t() const {
        return std::get<0>(*this);
    }
};

template <size_t N>
struct nd_range {
    id<N> global_size{};
    id<N> local_size{};

    nd_range() = default;

    constexpr explicit nd_range(id<N> global_size, id<N> local_size)
        : global_size(global_size), local_size(local_size)
    {}

    constexpr size_t get_global_size(size_t i) const {
        return global_size[i];
    }

    constexpr size_t get_local_size(size_t i) const {
        return local_size[i];
    }
};

template <size_t N>
struct nd_item : nd_range<N> {
    id<N> global_id{};
    id<N> local_id{};

    constexpr size_t get_global_id(size_t i) const {
        return global_id[i];
    }

    constexpr size_t get_local_id(size_t i) const {
        return local_id[i];
    }
};

template <size_t I, size_t N>
void _M_nd_range_for(id<N> const &size, id<N> &index, auto &&f) {
    if constexpr (I == N) {
        f(index);
    } else {
        for (index[I] = 0; index[I] < size[I]; index[I]++) {
            _M_nd_range_for<I + 1, N>(size, index, f);
        }
    }
}

template <size_t N>
void _M_nd_range_for(id<N> const &size, auto &&f) {
    id<N> index;
    _M_nd_range_for<0>(size, index, f);
}

struct handler {
    template <size_t N>
    void parallel_for(nd_range<N> range, auto &&f) {
        _M_nd_range_for(range.global_size, [&] (id<N> global_id) {
            nd_item<N> item;
            item.global_id = global_id;
            f(item);
        });
    }
};

struct queue {
    void submit(auto &&f) {
        handler h;
        f(h);
    }
};

namespace access {

enum class mode {
    read,
    write,
    read_write,
    discard_write,
    discard_read_write,
    atomic,
};

};

template <access::mode mode, class Buf, class T, size_t N>
struct accessor {
    Buf const &buf;

    explicit accessor(Buf const &buf) : buf(buf) {
    }

    inline decltype(auto) operator[](id<N> idx) const {
        return const_cast<Buf &>(buf)._M_at(idx);
    }
};

template <size_t N>
inline size_t _M_calc_product(id<N> const &size) {
    size_t ret = 1;
    for (int i = 0; i < N; i++) {
        ret *= size[i];
    }
    return ret;
}

template <size_t N>
inline size_t _M_linearize_id(id<N> const &size, id<N> const &idx) {
    size_t ret = 0;
    size_t term = 1;
    for (size_t i = 0; i < N; i++) {
        ret += term * idx[i];
        term *= size[i];
    }
    return ret;
}

template <class T, size_t N>
struct buffer {
    std::vector<T> _M_data;
    id<N> _M_size;

    explicit buffer(id<N> size)
        : _M_size(size), _M_data(_M_calc_product(size)) {
    }

    template <access::mode mode>
    auto get_access() const {
        return accessor<mode, buffer, T, N>(*this);
    }

    template <access::mode mode>
    auto get_access(sycl::handler &) const {
        return accessor<mode, buffer, T, N>(*this);
    }

    id<N> size() const {
        return _M_size;
    }

    T &_M_at(id<N> idx) {
        return _M_data.at(_M_linearize_id(_M_size, idx));
    }
};

}
}
ZENO_NAMESPACE_END
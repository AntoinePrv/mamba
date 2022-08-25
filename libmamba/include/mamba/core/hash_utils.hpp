#ifndef MAMBA_CORE_HASH_UTILS_HPP
#define MAMBA_CORE_HASH_UTILS_HPP

#include <cstddef>
#include <tuple>
#include <functional>
#include <utility>

namespace mamba::utils
{
    std::size_t combine_hash(std::size_t seed, std::size_t value);

    // Hash based on boost.container_hash
    // Distributed under the Boost Software License, Version 1.0.
    // https://www.boost.org/LICENSE_1_0.txt
    // https://github.com/boostorg/container_hash/blob/e00f53a69c9e6dba0af457dc22e41aedf86fd31a/include/boost/container_hash/detail/hash_tuple.hpp#L26
    //
    template <std::size_t I, typename T>
    std::size_t combine_hash_tuple(std::size_t seed, T const& t)
    {
        if constexpr (I < std::tuple_size_v<T>)
        {
            using Hasher = std::hash<std::tuple_element_t<I, T>>;
            seed = combine_hash(seed, Hasher{}(std::get<I>(t)));
            seed = combine_hash_tuple<I + 1>(seed, t);
        }
        return seed;
    }
}

namespace std
{
    template <typename... T>
    struct hash<std::tuple<T...>>
    {
        std::size_t operator()(std::tuple<T...> const& t) const
        {
            return mamba::utils::combine_hash_tuple<0>(0, t);
        }
    };
}

#endif

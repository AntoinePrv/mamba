#include <cstdint>

#include "mamba/core/hash_utils.hpp"

namespace mamba::utils
{

    // Hash functions copied from boost.pfr
    // https://github.com/boostorg/pfr/blob/8a8b5bc8d3ff673c4b278d145f6bf6973844d8e9/include/boost/pfr/detail/functional.hpp#L132
    //
    // Copyright (c) 2016-2022 Antony Polukhin
    //
    // Distributed under the Boost Software License, Version 1.0. (See accompanying
    // file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
    //
    // The implementation is based on Peter Dimov's proposal
    // http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2005/n1756.pdf
    // issue 6.18.
    //
    // This also contains public domain code from MurmurHash. From the
    // MurmurHash header:
    //
    // MurmurHash3 was written by Austin Appleby, and is placed in the public
    // domain. The author hereby disclaims copyright to this source code.
    namespace
    {
        constexpr std::uint32_t rotl(std::uint32_t x, std::uint32_t r) noexcept
        {
            return (x << r) | (x >> (32 - r));
        }

        [[maybe_unused]] constexpr std::uint32_t combine_hash_impl(std::uint32_t seed,
                                                                   std::uint32_t value) noexcept
        {
            constexpr std::uint32_t c1 = 0xcc9e2d51;
            constexpr std::uint32_t c2 = 0x1b873593;

            value *= c1;
            value = rotl(value, 15);
            value *= c2;

            seed ^= value;
            seed = rotl(seed, 13);
            seed = seed * 5 + 0xe6546b64;
            return seed;
        }

        [[maybe_unused]] constexpr std::uint64_t combine_hash_impl(std::uint64_t seed,
                                                                   std::uint64_t value) noexcept
        {
            constexpr std::uint64_t m = 0xc6a4a7935bd1e995ULL;
            constexpr int r = 47;

            value *= m;
            value ^= value >> r;
            value *= m;

            seed ^= value;
            seed *= m;

            // Completely arbitrary number, to prevent 0's from hashing to 0.
            seed += 0xe6546b64;
            return seed;
        }

    }

    constexpr std::size_t combine_hash(std::size_t seed, std::size_t value)
    {
        return combine_hash_impl(seed, value);
    }
}

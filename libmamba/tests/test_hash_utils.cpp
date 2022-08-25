#include <cstddef>
#include <string>

#include <gtest/gtest.h>

#include "mamba/core/hash_utils.hpp"

namespace mamba::utils

{
    class HashCombine : public testing::TestWithParam<std::tuple<std::size_t, std::size_t>>
    {
    };

    TEST_P(HashCombine, test_hash_combine)
    {
        auto const [seed, value] = GetParam();
        auto const combined = combine_hash(seed, value);
        EXPECT_NE(combined, seed);
        EXPECT_NE(combined, value);
    }

    INSTANTIATE_TEST_SUITE_P(hash,
                             HashCombine,
                             testing::Values(std::tuple(0, 0),
                                             std::tuple(0, 15),
                                             std::tuple(17, 17),
                                             std::tuple(32, 0)));

    TEST(hash, test_hash_tuple)
    {
        {
            using Hasher = std::hash<std::tuple<int, double>>;
            EXPECT_EQ(Hasher{}({ 1, 2.3 }), Hasher{}({ 1, 2.3 }));
        }
        {
            using Hasher = std::hash<std::tuple<int, int, std::string>>;
            EXPECT_EQ(Hasher{}({ 1, 3, std::string("hello") }),
                      Hasher{}({ 1, 3, std::string("hello") }));
        }
        {
            using Hasher = std::hash<std::tuple<int, int, const char*, int, int, const char*>>;
            EXPECT_EQ(Hasher{}({ 1, 3, "hello", 1, 3, "hello" }),
                      Hasher{}({ 1, 3, "hello", 1, 3, "hello" }));
        }
    }

    TEST(hash, test_hash_symetric)
    {
        {
            using Hasher = std::hash<std::tuple<int, int>>;
            EXPECT_NE(Hasher{}({ 1, 3 }), Hasher{}({ 3, 1 }));
        }
        {
            using Hasher = std::hash<std::tuple<std::string, std::string>>;
            EXPECT_NE(Hasher{}({ "hello", "world" }), Hasher{}({ "world", "hello" }));
        }
        {
            using Hasher = std::hash<std::tuple<int, int, int>>;
            EXPECT_NE(Hasher{}({ 1, 2, 3 }), Hasher{}({ 3, 2, 1 }));
            EXPECT_NE(Hasher{}({ 1, 2, 3 }), Hasher{}({ 1, 3, 2 }));
            EXPECT_NE(Hasher{}({ 3, 2, 1 }), Hasher{}({ 1, 3, 2 }));
        }
    }

}

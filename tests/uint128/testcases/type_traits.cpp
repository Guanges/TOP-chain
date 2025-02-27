#include <type_traits>

#include <gtest/gtest.h>

#include "uint128_t.h"

TEST(Type_Traits, is_arithmetic){
    EXPECT_EQ(std::is_arithmetic <top::xstake::uint128_t>::value, true);
}

TEST(Type_Traits, is_integral){
    EXPECT_EQ(std::is_integral <top::xstake::uint128_t>::value, true);
}

TEST(Type_Traits, is_unsigned){
    EXPECT_EQ(std::is_unsigned <top::xstake::uint128_t>::value, true);
}

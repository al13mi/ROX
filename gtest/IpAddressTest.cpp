#include <gtest/gtest.h>
#include "Network/IpAddressV4.h"

TEST(IpAddressTest, TestBitset)
{
    Network::IpAddressV4 address(1);
    ASSERT_TRUE(address.getBitset()[0]);
    ASSERT_FALSE(address.getBitset()[1]);
}

TEST(IpAddressTest, TestData)
{
    Network::IpAddressV4 address(0xAABBCCDD);
    ASSERT_EQ(address.data.byte[0], 0xDD);
    ASSERT_EQ(address.data.byte[1], 0xCC);
    ASSERT_EQ(address.data.byte[2], 0xBB);
    ASSERT_EQ(address.data.byte[3], 0xAA);
    ASSERT_EQ(address.data.word, 0xAABBCCDD);
}
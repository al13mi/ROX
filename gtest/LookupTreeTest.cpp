#include <gtest/gtest.h>
#include "System/LookupTree.h"

class LookupTreeTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {

    }

    virtual void TearDown()
    {

    }

    System::LookupTree tree;
};

TEST_F(LookupTreeTest, InstantiationTest)
{
    System::LookupTree tree;
}

TEST_F(LookupTreeTest, nextHop)
{
    Network::IpAddressV4 address(5);
    System::Node node(address);
    ASSERT_EQ(node.nextHop.data.word, address.data.word);
}

TEST_F(LookupTreeTest, insertNoExplosions)
{
    Network::IpAddressV4 address(0xFFFFFFFF);
    tree.insert(address, 24, address);
    Network::IpAddressV4 address2(0x0FFFFFFF);
    tree.insert(address2, 24, address);
}

TEST_F(LookupTreeTest, getMatchingPrefix)
{
    Network::IpAddressV4 route(1285860864);
    Network::IpAddressV4 nextHop(1285860980);
    Network::IpAddressV4 address(1285860980);

    Network::IpAddressV4 result;

    tree.insert(route, 24, nextHop);
    tree.getMatchingPrefix(address, result);
    ASSERT_EQ(result.data.word, nextHop.data.word);

    route.data.word = 1285861064;
    address.data.word = 1285861064;
    result.data.word = 0;

    tree.insert(route, 28, nextHop);
    tree.getMatchingPrefix(address, result);
    ASSERT_EQ(result.data.word, nextHop.data.word);
}
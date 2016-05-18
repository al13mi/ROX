#include <gtest/gtest.h>
#include "Network/RouteTable.h"
class RouterTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {

    }

    virtual void TearDown()
    {

    }

    Network::RouteTable routeTable;
};


TEST_F(RouterTest, TestHostMask)
{
    uint64_t res = Network::getHostMask(0xFFFFFFFF, 24);
    ASSERT_EQ(0xFFFFFF00, res);
    res = Network::getHostMask(0xFFFFFFFF, 16);
    ASSERT_EQ(0xFFFF0000, res);
    res = Network::getHostMask(0xFFFFFFFF, 8);
    ASSERT_EQ(0xFF000000, res);
    res = Network::getHostMask(0xFFFFFFFF, 32);
    ASSERT_EQ(0xFFFFFFFF, res);
    res = Network::getHostMask(3232243726ul, 30);
    ASSERT_EQ(3232243724ul, res);
}

/**
TEST_F(RouterTest, TestInsert)
{
    uint32_t address = 0xFFFFFFFF;
    uint32_t size = 0xFFFFFFFF;
    routeTable.insertRoute(address, size, address);
    uint32_t nextHop = 0;
    routeTable.findNextHop(1, nextHop);
    ASSERT_NE(nextHop, address);

}
 **/
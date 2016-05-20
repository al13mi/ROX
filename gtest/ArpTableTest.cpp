#include <gtest/gtest.h>
#include "Network/ArpTable.h"

class ArpTableTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {

    }

    virtual void TearDown()
    {

    }

    Network::ArpTable arpTable;
};

TEST_F(ArpTableTest, InstantiationTest)
{
    Network::ArpTable arpTable;
}

TEST_F(ArpTableTest, ArpLookupTest)
{
    Network::IpAddressV4 address(5);
    uint64_t mac = 0xFF;
    uint64_t res = 0;
    arpTable.insertArpEntry(mac, address);
    arpTable.findArpEntry(res, address);

    ASSERT_EQ(res, mac);
}


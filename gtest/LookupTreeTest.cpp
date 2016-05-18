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
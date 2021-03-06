#include <stdio.h>
#include <gtest/gtest.h>
#include "Sum.h"

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(DemoTestGroup, SumTest)
{
    ASSERT_EQ(3, sum(1, 2));
    ASSERT_EQ(-4, sum(1, -5));
    ASSERT_EQ(4, sum(-1, 5));
}
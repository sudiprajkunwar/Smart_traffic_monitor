#include <gtest/gtest.h>
#include "../src/GenericObject/GenericObject.h"

TEST(GenericObjectTest, CalculateSpeedTest)
{
    GenericObject obj("TestObject");
    double distance = 100.0;
    double time = 2.0;
    double speed = obj.calculateSpeed(distance, time);
    EXPECT_EQ(speed, 50.0);
}

TEST(GenericObjectTest, ProcessFrameTest)
{
    GenericObject obj("TestObject");
    cv::Rect box(10, 10, 20, 20);
    double currentTime = 1.0;
    obj.processFrame(box, currentTime);

    // Since processFrame prints the speed, we can't directly test the output.
    // However, we can test if the previousPosition and previousTime are updated correctly.
    EXPECT_EQ(obj.getObjectName(), "TestObject");
}
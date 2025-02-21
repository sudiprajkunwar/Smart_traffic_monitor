#include <gtest/gtest.h>
#include "../src/LicensePlateDetector/LicensePlateDetector.h"

TEST(LicensePlateDetectorTest, CalculateBoundingBoxTest)
{
    LicensePlateDetector detector("", "", "", "");
    float data[] = {0.5, 0.5, 0.2, 0.2}; // centerX, centerY, width, height
    int frameWidth = 1000;
    int frameHeight = 1000;
    cv::Rect box = detector.calculateBoundingBox(data, frameWidth, frameHeight);

    EXPECT_EQ(box.x, 400);
    EXPECT_EQ(box.y, 400);
    EXPECT_EQ(box.width, 200);
    EXPECT_EQ(box.height, 200);
}

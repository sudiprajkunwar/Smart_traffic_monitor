#include <gtest/gtest.h>
#include "../src/LicensePlateDetector/LicensePlateDetector.h"
#include "../src/AlertSystem/AlertSystem.h"
#include "../src/StationNotifier/StationNotifier.h"

TEST(LicensePlateDetectorTest, CalculateBoundingBoxTest)
{
    // Create a dummy AlertSystem instance
    AlertSystem alertSystem(new StationNotifier());

    // Create LicensePlateDetector instance with a valid AlertSystem reference
    LicensePlateDetector detector("", "", "", "", alertSystem);

    float data[] = {0.5f, 0.5f, 0.2f, 0.2f}; // {centerX, centerY, width, height}
    int frameWidth = 1000;
    int frameHeight = 1000;

    cv::Rect box = detector.calculateBoundingBox(data, frameWidth, frameHeight);

    // Expected values:
    // centerX = 0.5 * 1000 = 500
    // centerY = 0.5 * 1000 = 500
    // width = 0.2 * 1000 = 200
    // height = 0.2 * 1000 = 200
    // x = centerX - width / 2 = 500 - 100 = 400
    // y = centerY - height / 2 = 500 - 100 = 400

    EXPECT_EQ(box.x, 400);
    EXPECT_EQ(box.y, 400);
    EXPECT_EQ(box.width, 200);
    EXPECT_EQ(box.height, 200);
}

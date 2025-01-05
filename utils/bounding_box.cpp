#include <opencv2/opencv.hpp>
#include <algorithm>

cv::Rect calculateBoundingBox(const float *data, int frameWidth, int frameHeight)
{
    int centerX = static_cast<int>(data[0] * frameWidth);
    int centerY = static_cast<int>(data[1] * frameHeight);
    int width = static_cast<int>(data[2] * frameWidth);
    int height = static_cast<int>(data[3] * frameHeight);

    int x = centerX - width / 2;
    int y = centerY - height / 2;

    x = std::max(0, x);
    y = std::max(0, y);
    width = std::min(frameWidth - x, width);
    height = std::min(frameHeight - y, height);

    return cv::Rect(x, y, width, height);
}

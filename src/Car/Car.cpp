#include "Car.h"
#include <cmath>
#include <iostream>

Car::Car() : previousPosition(cv::Point(0, 0)), previousTime(0.0) {}

double Car::calculateSpeed(double distance, double time) const
{
    return distance / time;
}

void Car::processFrame(cv::Rect &box, double currentTime)
{
    int centerX = box.x + box.width / 2;
    int centerY = box.y + box.height / 2;

    if (previousPosition.x != 0 && previousPosition.y != 0)
    {
        double distance = std::sqrt(std::pow(centerX - previousPosition.x, 2) + std::pow(centerY - previousPosition.y, 2));
        double time = currentTime - previousTime;

        if (time > 0)
        {
            double speed = calculateSpeed(distance, time);
            std::cout << "Speed: " << speed << " pixels per second" << std::endl;
        }
    }

    previousPosition = cv::Point(centerX, centerY);
    previousTime = currentTime;
}

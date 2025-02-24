#include <cmath>
#include <iostream>

#include "../GenericObject/GenericObject.h"

// Constructor to initialize the object name, position, and time
GenericObject::GenericObject(const std::string &name) : objectName(name), previousPosition(cv::Point(0, 0)), previousTime(0.0), speed(0.0) {}

// Function to calculate speed
double GenericObject::calculateSpeed(double distance, double time) const
{
    return distance / time;
}

// Process each frame and track the object's position
void GenericObject::processFrame(cv::Rect &box, double currentTime)
{
    int centerX = box.x + box.width / 2;
    int centerY = box.y + box.height / 2;

    if (previousPosition.x != 0 && previousPosition.y != 0)
    {
        double distance = std::sqrt(std::pow(centerX - previousPosition.x, 2) + std::pow(centerY - previousPosition.y, 2));
        double time = currentTime - previousTime;

        if (time > 0)
        {
            speed = calculateSpeed(distance, time);
            Logger::getInstance().log(LogLevel::INFO, "Speed of " + objectName + ": " + std::to_string(speed) + " pixels per second");
        }
    }

    previousPosition = cv::Point(centerX, centerY);
    previousTime = currentTime;
}

std::string GenericObject::getObjectName() const
{
    return objectName;
}

double GenericObject::getSpeed() const
{
    return speed;
}

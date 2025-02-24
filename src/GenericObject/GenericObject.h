#ifndef GENERIC_OBJECT_H
#define GENERIC_OBJECT_H

#include <string>
#include <opencv2/opencv.hpp>
#include "../Logger/Logger.h"

class GenericObject
{
private:
    cv::Point previousPosition;
    double previousTime = 0.0;
    std::string objectName;
    double speed;

public:
    // Default constructor
    GenericObject() : objectName(""), speed(0.0) {}

    explicit GenericObject(const std::string &name);

    // method to process frames and track object's position
    void processFrame(cv::Rect &box, double currentTime);
    double calculateSpeed(double distance, double time) const;

    std::string getObjectName() const;
    double getSpeed() const;
};

#endif // GENERIC_OBJECT_H

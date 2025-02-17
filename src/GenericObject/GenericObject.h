#ifndef GENERIC_OBJECT_H
#define GENERIC_OBJECT_H

#include <opencv2/opencv.hpp>
#include <string>

class GenericObject
{
protected:
    cv::Point previousPosition;
    double previousTime = 0.0;
    std::string objectName;
    double calculateSpeed(double distance, double time) const;

public:
    explicit GenericObject(const std::string &name);

    // method to process frames and track object's position
    void processFrame(cv::Rect &box, double currentTime);

    std::string getObjectName() const;
};

#endif // GENERIC_OBJECT_H

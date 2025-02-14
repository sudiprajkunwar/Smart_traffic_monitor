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

public:
    GenericObject(std::string name);
    virtual ~GenericObject() = default;

    // Function to calculate speed
    virtual double calculateSpeed(double distance, double time) const;

    // Virtual method to process frames and track object's position
    virtual void processFrame(cv::Rect &box, double currentTime);

    std::string getObjectName() const;
};

#endif // GENERIC_OBJECT_H

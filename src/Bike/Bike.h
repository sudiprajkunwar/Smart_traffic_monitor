#ifndef BIKE_H
#define BIKE_H

#include <opencv2/opencv.hpp>
#include "../GenericObject/GenericObject.h"

class Bike : public GenericObject
{
public:
    // Constructor for Bike class
    Bike();

    // Override the processFrame method from GenericObject
    void processFrame(cv::Rect &box, double currentTime) override;
};

#endif // BIKE_H

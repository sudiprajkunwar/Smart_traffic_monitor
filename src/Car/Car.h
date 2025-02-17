#ifndef CAR_H
#define CAR_H

#include <opencv2/opencv.hpp>
#include "../GenericObject/GenericObject.h"

class Car : public GenericObject
{
public:
    // Constructor for Car class
    Car();

    // Override the processFrame method from GenericObject
    void processFrame(cv::Rect &box, double currentTime) override;
};

#endif // CAR_H

#include "Bike.h"
#include <cmath>
#include <iostream>
#include "../GenericObject/GenericObject.h"

Bike::Bike() : GenericObject("Bike") {}

void Bike::processFrame(cv::Rect &box, double currentTime)
{
    // Call the base class implementation
    GenericObject::processFrame(box, currentTime);

    // Additional behavior specific to Car
    std::cout << "Bike is being tracked!" << std::endl;
}

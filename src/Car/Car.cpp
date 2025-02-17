#include "Car.h"
#include <cmath>
#include <iostream>
#include "../GenericObject/GenericObject.h"

Car::Car() : GenericObject("Car") {}

void Car::processFrame(cv::Rect &box, double currentTime)
{
    // Call the base class implementation
    GenericObject::processFrame(box, currentTime);

    // Additional behavior specific to Car
    std::cout << "Car is being tracked!" << std::endl;
}

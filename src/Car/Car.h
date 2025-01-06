#ifndef CAR_H
#define CAR_H

#include <opencv2/opencv.hpp>
#include "../Entity/Entity.h"

class Car : public Entity
{
public:
    // Constructor for Car
    Car();

    // Function to calculate speed
    double calculateSpeed(double distance, double time) const;

    // Function to process each frame and track the car's position
    void processFrame(cv::Rect &box, double currentTime) override;
};

#endif // CAR_H

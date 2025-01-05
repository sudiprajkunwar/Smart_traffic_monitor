#ifndef CAR_H
#define CAR_H

#include <opencv2/opencv.hpp>

class Car
{
public:
    cv::Point previousPosition;
    double previousTime = 0.0;

    // Constructor for Car
    Car();

    // Function to calculate speed
    double calculateSpeed(double distance, double time) const;

    // Function to process each frame and track the car's position
    void processFrame(cv::Rect &box, double currentTime);
};

#endif // CAR_H

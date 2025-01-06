#ifndef ENTITY_H
#define ENTITY_H

#include <opencv2/opencv.hpp>

class Entity
{
protected:
    cv::Point previousPosition;
    double previousTime = 0.0;

public:
    virtual ~Entity() = default;
    // Pure virtual method to process frames
    virtual void processFrame(cv::Rect &box, double currentTime) = 0;
};

#endif // ENTITY_H

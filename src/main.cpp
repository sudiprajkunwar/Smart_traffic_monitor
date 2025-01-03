#include <opencv2/opencv.hpp>
#include <iostream>
#include "LicensePlateDetector/LicensePlateDetector.h"

int main()
{
    std::string videoFile = "../data/demo.mp4";
    std::string modelConfiguration = "../models/yolov4.cfg";
    std::string modelWeights = "../models/yolov4.weights";
    std::string classesFile = "../models/classes.names";

    // Initialize LicensePlateDetector
    LicensePlateDetector detector(modelConfiguration, modelWeights, classesFile);
    if (!detector.initialize())
    {
        std::cerr << "Initialization failed!" << std::endl;
        return -1;
    }

    cv::VideoCapture cap(videoFile);
    if (!cap.isOpened())
    {
        std::cerr << "Error opening video file!" << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (cap.read(frame))
    {
        detector.processFrame(frame);
        cv::imshow("Detected License Plates", frame);
        if (cv::waitKey(30) == 'q')
            break;
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}

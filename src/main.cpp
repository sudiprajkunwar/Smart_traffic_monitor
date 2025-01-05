#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <tesseract/baseapi.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "./LicensePlateDetector/LicensePlateDetector.h"

int main()
{
    std::string videoFile = "../data/demo.mp4";
    std::string modelConfiguration = "../models/yolov4.cfg";
    std::string modelWeights = "../models/yolov4.weights";
    std::string classesFile = "../models/classes.names";

    LicensePlateDetector detector(videoFile, modelConfiguration, modelWeights, classesFile);
    detector.initialize();
    detector.processFrames();

    return 0;
}

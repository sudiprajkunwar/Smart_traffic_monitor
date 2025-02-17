#ifndef LICENSE_PLATE_DETECTOR_HPP
#define LICENSE_PLATE_DETECTOR_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <tesseract/baseapi.h>
#include <vector>
#include <string>
#include "./OCRProcessor.cpp"
#include "../GenericObject/GenericObject.h"

class LicensePlateDetector
{
public:
    LicensePlateDetector(const std::string &videoFile, const std::string &modelConfiguration,
                         const std::string &modelWeights, const std::string &classesFile);

    void initialize();
    void processFrames();

private:
    std::string videoFile;
    std::string modelConfiguration;
    std::string modelWeights;
    std::string classesFile;

    cv::dnn::Net net;
    std::vector<std::string> classNames;

    void loadClassNames();
    void processDetection(cv::Mat &frame);
    cv::Rect calculateBoundingBox(const float *data, int frameWidth, int frameHeight);
};

#endif // LICENSE_PLATE_DETECTOR_HPP

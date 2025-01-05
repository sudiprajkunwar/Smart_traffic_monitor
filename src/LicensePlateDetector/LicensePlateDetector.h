#ifndef LICENSE_PLATE_DETECTOR_HPP
#define LICENSE_PLATE_DETECTOR_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <tesseract/baseapi.h>
#include <vector>
#include <string>
#include "../Car/Car.h"

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
    tesseract::TessBaseAPI ocr;
    Car car;
    std::vector<std::string> classNames;

    void loadClassNames();
    void processDetection(cv::Mat &frame);
};

#endif // LICENSE_PLATE_DETECTOR_HPP

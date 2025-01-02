#ifndef LICENSE_PLATE_DETECTOR_H
#define LICENSE_PLATE_DETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <tesseract/baseapi.h>
#include <vector>
#include <string>

class LicensePlateDetector
{
public:
    LicensePlateDetector(const std::string &modelConfig, const std::string &modelWeights, const std::string &classesFile);
    ~LicensePlateDetector();

    bool initialize();
    void processFrame(cv::Mat &frame);

private:
    cv::dnn::Net net;
    tesseract::TessBaseAPI ocr;
    std::vector<std::string> classNames;

    bool loadClassNames(const std::string &classesFile);
    void preprocessForOCR(const cv::Mat &input, cv::Mat &output); // Moved directly here
    void performOCR(const cv::Mat &licensePlate, cv::Mat &frame, const cv::Rect &box);
};

#endif

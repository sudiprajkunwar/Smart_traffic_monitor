#ifndef LICENSE_PLATE_DETECTOR_HPP
#define LICENSE_PLATE_DETECTOR_HPP

#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <tesseract/baseapi.h>
#include "./OCRProcessor.cpp"
#include "../GenericObject/GenericObject.h"
#include "../AlertSystem/AlertSystem.h"
#include "../StationNotifier/StationNotifier.h"

class LicensePlateDetector
{

private:
    std::string videoFile;
    std::string modelConfiguration;
    std::string modelWeights;
    std::string classesFile;
    AlertSystem *alertSystem;
    std::map<std::string, GenericObject> trackedVehicles; // Track vehicles by license plate

    cv::dnn::Net net;
    std::vector<std::string> classNames;

    void processDetection(cv::Mat &frame);

public:
    LicensePlateDetector(const std::string &videoFile, const std::string &modelConfiguration,
                         const std::string &modelWeights, const std::string &classesFile, const AlertSystem &alertSystem);

    void initialize();
    void processFrames();
    void loadClassNames();
    std::unique_ptr<GenericObject> genericObject;

    cv::Rect calculateBoundingBox(const float *data, int frameWidth, int frameHeight);

    ~LicensePlateDetector();
};

#endif // LICENSE_PLATE_DETECTOR_HPP

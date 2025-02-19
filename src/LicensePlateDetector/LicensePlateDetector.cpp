#include "LicensePlateDetector.h"
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <tesseract/baseapi.h>
#include <iostream>
#include <fstream>
#include <cmath>

#define CLASSES_DATA_AFTER_INDEX 5

LicensePlateDetector::LicensePlateDetector(const std::string &videoFile, const std::string &modelConfiguration,
                                           const std::string &modelWeights, const std::string &classesFile)
    : videoFile(videoFile), modelConfiguration(modelConfiguration),
      modelWeights(modelWeights), classesFile(classesFile) {}

cv::Rect LicensePlateDetector::calculateBoundingBox(const float *data, int frameWidth, int frameHeight)
{
    int centerX = static_cast<int>(data[0] * frameWidth);
    int centerY = static_cast<int>(data[1] * frameHeight);
    int width = static_cast<int>(data[2] * frameWidth);
    int height = static_cast<int>(data[3] * frameHeight);

    int x = centerX - width / 2;
    int y = centerY - height / 2;

    x = std::max(0, x);
    y = std::max(0, y);
    width = std::min(frameWidth - x, width);
    height = std::min(frameHeight - y, height);

    return cv::Rect(x, y, width, height);
}

void LicensePlateDetector::initialize()
{
    net = cv::dnn::readNetFromDarknet(modelConfiguration, modelWeights);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    loadClassNames();
}

void LicensePlateDetector::loadClassNames()
{
    std::ifstream ifs(classesFile);
    if (!ifs.is_open())
    {
        std::cerr << "Error opening classes file!" << std::endl;
        exit(-1);
    }

    std::string line;
    while (std::getline(ifs, line))
    {
        classNames.emplace_back(line);
    }
}

void LicensePlateDetector::processFrames()
{
    cv::VideoCapture cap(videoFile);
    if (!cap.isOpened())
    {
        std::cerr << "Error opening video file!" << std::endl;
        exit(-1);
    }

    cv::Mat frame;
    while (cap.read(frame))
    {
        processDetection(frame); // Call the processDetection function

        // Display the frame with detections
        cv::imshow("Detected License Plates", frame);
        if (cv::waitKey(30) == 'q')
            break;
    }
    cap.release();
    cv::destroyAllWindows();
}

void LicensePlateDetector::processDetection(cv::Mat &frame)
{
    cv::Mat blob = cv::dnn::blobFromImage(frame, 1 / 255.0, cv::Size(416, 416), cv::Scalar(0, 0, 0), true, false);
    net.setInput(blob);

    std::vector<std::string> layerNames = net.getUnconnectedOutLayersNames();
    std::vector<cv::Mat> detections;
    net.forward(detections, layerNames);

    double currentTime = cv::getTickCount() / cv::getTickFrequency(); // Get the current time for speed calculation

    for (const auto &output : detections)
    {
        for (int i = 0; i < output.rows; ++i)
        {
            const float *data = reinterpret_cast<const float *>(output.ptr(i));
            float confidence = data[4];

            if (confidence > 0.5)
            {
                int classId = -1;
                float maxConfidence = 0.0;

                for (int j = CLASSES_DATA_AFTER_INDEX; j < output.cols; ++j)
                {
                    if (data[j] > maxConfidence)
                    {
                        maxConfidence = data[j];
                        classId = j - CLASSES_DATA_AFTER_INDEX;
                    }
                }

                if (maxConfidence > 0.5 && classNames[classId] == "license_plate")
                {

                    cv::Rect box = calculateBoundingBox(data, frame.cols, frame.rows);

                    std::unique_ptr<GenericObject> genericObject = std::make_unique<GenericObject>("Car");

                    // Process the tracked object
                    genericObject->processFrame(box, currentTime);

                    if (box.width > 0 && box.height > 0)
                    {
                        cv::Mat licensePlate = frame(box);

                        OCRProcessor ocrProcessor;
                        std::string plateText = ocrProcessor.extractText(licensePlate);

                        // Display results
                        cv::rectangle(frame, box, cv::Scalar(0, 255, 0), 2);
                        cv::putText(frame, plateText, cv::Point(box.x, box.y - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
                        std::cout << "Lincence plate number: " << plateText << "\n";
                    }
                }
            }
        }
    }
}

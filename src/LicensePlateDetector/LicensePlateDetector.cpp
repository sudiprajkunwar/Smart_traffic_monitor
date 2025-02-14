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
      modelWeights(modelWeights), classesFile(classesFile), net(), ocr() {}

void LicensePlateDetector::initialize()
{
    net = cv::dnn::readNetFromDarknet(modelConfiguration, modelWeights);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    ocr.Init(NULL, "eng");

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
                    int centerX = static_cast<int>(data[0] * frame.cols);
                    int centerY = static_cast<int>(data[1] * frame.rows);
                    int width = static_cast<int>(data[2] * frame.cols);
                    int height = static_cast<int>(data[3] * frame.rows);
                    int x = centerX - width / 2;
                    int y = centerY - height / 2;

                    x = std::max(0, x);
                    y = std::max(0, y);
                    width = std::min(frame.cols - x, width);
                    height = std::min(frame.rows - y, height);

                    cv::Rect box(x, y, width, height);

                    std::unique_ptr<GenericObject> genericObject;

                    genericObject = std::make_unique<Car>();

                    // Process the tracked object
                    car.processFrame(box, currentTime);

                    if (box.width > 0 && box.height > 0)
                    {
                        cv::Mat licensePlate = frame(box);

                        // Preprocess for OCR
                        cv::Mat gray;
                        cv::cvtColor(licensePlate, gray, cv::COLOR_BGR2GRAY);
                        cv::threshold(gray, gray, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

                        // Perform OCR
                        ocr.SetImage(gray.data, gray.cols, gray.rows, 1, gray.step);
                        std::string plateText = ocr.GetUTF8Text();

                        // Display results
                        cv::rectangle(frame, box, cv::Scalar(0, 255, 0), 2);
                        cv::putText(frame, plateText, cv::Point(x, y - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
                        std::cout << "Lincence plate number: " << plateText << "\n";
                    }
                }
            }
        }
    }
}

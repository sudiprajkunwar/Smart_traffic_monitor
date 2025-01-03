#include "LicensePlateDetector.h"
#include <fstream>
#include <iostream>

LicensePlateDetector::LicensePlateDetector(const std::string &modelConfig, const std::string &modelWeights, const std::string &classesFile)
    : ocr(), net(cv::dnn::readNetFromDarknet(modelConfig, modelWeights))
{
    loadClassNames(classesFile);
}

LicensePlateDetector::~LicensePlateDetector()
{
    ocr.End();
}

bool LicensePlateDetector::initialize()
{
    if (ocr.Init(NULL, "eng"))
    {
        std::cerr << "Error initializing Tesseract!" << std::endl;
        return false;
    }
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    return true;
}

bool LicensePlateDetector::loadClassNames(const std::string &classesFile)
{
    std::ifstream ifs(classesFile);
    if (!ifs.is_open())
    {
        std::cerr << "Error opening classes file!" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(ifs, line))
    {
        classNames.emplace_back(line);
    }
    return true;
}

void LicensePlateDetector::processFrame(cv::Mat &frame)
{
    cv::Mat blob = cv::dnn::blobFromImage(frame, 1 / 255.0, cv::Size(416, 416), cv::Scalar(0, 0, 0), true, false);
    net.setInput(blob);

    // Get output layer names
    std::vector<std::string> layerNames = net.getUnconnectedOutLayersNames();
    std::vector<cv::Mat> detections;
    net.forward(detections, layerNames);

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
                for (int j = 5; j < output.cols; ++j)
                {
                    if (data[j] > maxConfidence)
                    {
                        maxConfidence = data[j];
                        classId = j - 5;
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

                    // Ensure bounding box is within bounds
                    x = std::max(0, x);
                    y = std::max(0, y);
                    width = std::min(frame.cols - x, width);
                    height = std::min(frame.rows - y, height);

                    cv::Rect box(x, y, width, height);
                    if (box.width > 0 && box.height > 0)
                    {
                        cv::Mat licensePlate = frame(box);
                        performOCR(licensePlate, frame, box);
                    }
                }
            }
        }
    }
}

void LicensePlateDetector::performOCR(const cv::Mat &licensePlate, cv::Mat &frame, const cv::Rect &box)
{
    // Preprocess the license plate image for OCR (convert to grayscale and threshold)
    cv::Mat gray;
    preprocessForOCR(licensePlate, gray);

    ocr.SetImage(gray.data, gray.cols, gray.rows, 1, gray.step);
    std::string plateText = ocr.GetUTF8Text();

    std::cout << "Detected License Plate Text: " << plateText << std::endl;
    cv::rectangle(frame, box, cv::Scalar(0, 255, 0), 2);
    cv::putText(frame, plateText, cv::Point(box.x, box.y - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
}

void LicensePlateDetector::preprocessForOCR(const cv::Mat &input, cv::Mat &output)
{
    cv::cvtColor(input, output, cv::COLOR_BGR2GRAY);                            // Convert to grayscale
    cv::threshold(output, output, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU); // Apply Otsu thresholding
}

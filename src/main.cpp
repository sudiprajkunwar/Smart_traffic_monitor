#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <tesseract/baseapi.h>
#include <iostream>
#include <vector>
#include <fstream>

int main()
{
    std::string videoFile = "demo.mp4";
    std::string modelConfiguration = "yolov4.cfg";
    std::string modelWeights = "yolov4.weights";
    std::string classesFile = "classes.names";

    std::vector<std::string> classNames;
    std::ifstream ifs(classesFile);

    if (!ifs.is_open())
    {
        std::cerr << "Error opening classes file!" << std::endl;
        return -1;
    }
    std::string line;
    while (std::getline(ifs, line))
        classNames.push_back(line);

    // Load YOLO model
    cv::dnn::Net net = cv::dnn::readNetFromDarknet(modelConfiguration, modelWeights);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    // Open the video file
    cv::VideoCapture cap(videoFile);
    if (!cap.isOpened())
    {
        std::cerr << "Error opening video file!" << std::endl;
        return -1;
    }

    // Initialize Tesseract OCR
    tesseract::TessBaseAPI ocr;
    if (ocr.Init(NULL, "eng"))
    {
        std::cerr << "Error initializing Tesseract!" << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (cap.read(frame))
    {
        cv::Mat blob = cv::dnn::blobFromImage(frame, 1 / 255.0, cv::Size(416, 416), cv::Scalar(0, 0, 0), true, false);
        net.setInput(blob);

        // Get output layer names
        std::vector<std::string> layerNames = net.getUnconnectedOutLayersNames();
        std::vector<cv::Mat> detections;
        net.forward(detections, layerNames);

        // Process detections
        for (const auto &output : detections)
        {
            for (int i = 0; i < output.rows; ++i)
            {
                const float *data = reinterpret_cast<const float *>(output.ptr(i));
                float confidence = data[4]; // Confidence score

                if (confidence > 0.5) // Confidence threshold
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

                        // Create bounding box
                        cv::Rect box(x, y, width, height);
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
                            std::cout << "Detected License Plate Text: " << plateText << std::endl;

                            // Display results
                            cv::rectangle(frame, box, cv::Scalar(0, 255, 0), 2);
                            cv::putText(frame, plateText, cv::Point(x, y - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
                        }
                    }
                }
            }
        }

        // Display the frame with detections
        cv::imshow("Detected License Plates", frame);
        if (cv::waitKey(30) == 'q')
            break;
    }

    cap.release();
    cv::destroyAllWindows();
    ocr.End();

    return 0;
}

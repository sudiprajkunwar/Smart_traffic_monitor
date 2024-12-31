#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>
#include <vector>
#include <fstream>

int main()
{
    std::string videoFile = "demo.mp4";            // Path to the input video
    std::string modelConfiguration = "yolov4.cfg"; // Path to YOLO config file
    std::string modelWeights = "yolov4.weights";   // Path to YOLO weights file
    std::string classesFile = "classes.names";     // Path to classes names file

    // Load class names
    std::vector<std::string> classNames;
    std::ifstream ifs(classesFile.c_str());
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

                        cv::Rect box(x, y, width, height);
                        cv::rectangle(frame, box, cv::Scalar(0, 255, 0), 2);
                        cv::putText(frame, "License Plate", cv::Point(x, y - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);

                        // Crop the detected license plate region
                        cv::Mat licensePlate = frame(box);

                        // Display the cropped license plate
                        cv::imshow("License Plate", licensePlate);
                    }
                }
            }
        }

        // Display the frame with detections
        cv::imshow("Detected License Plates", frame);
        if (cv::waitKey(30) == 'q') // Exit if 'q' is pressed
            break;
    }

    cap.release();           // Release video resources
    cv::destroyAllWindows(); // Close all OpenCV windows

    return 0;
}

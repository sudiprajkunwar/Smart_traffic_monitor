#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <tesseract/baseapi.h>

// Strategy Pattern (for OCR Processing)
// Decouple OCR logic by creating a separate OCR handler.

class OCRProcessor
{
private:
    tesseract::TessBaseAPI ocr;

public:
    OCRProcessor() { ocr.Init(NULL, "eng"); }
    std::string extractText(const cv::Mat &image)
    {
        cv::Mat gray;
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
        cv::threshold(gray, gray, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
        ocr.SetImage(gray.data, gray.cols, gray.rows, 1, gray.step);
        return ocr.GetUTF8Text();
    }
};

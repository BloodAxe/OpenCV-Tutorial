//
//  DrawingCanvas.cpp
//  OpenCV Tutorial
//
//  Created by BloodAxe on 9/4/12.
//
//

#include "DrawingCanvas.h"

#define kStageGrayscale       "Gray"
#define kStageBilateralFilter "Filtered"
#define kStageThreshold       "Threshold"
#define kStateResult          "Result"

DrawingCanvasSample::DrawingCanvasSample()
: m_bilateralSigmaThreshold(100)
, m_bilateralColorThreshold(100)
, m_bilateralD(5)
, m_thresholdBlockSize(9)
, m_thresholdC(5)
{
    std::vector<std::string> stages;
    
    stages.push_back(kStageGrayscale);
    stages.push_back(kStageBilateralFilter);
    stages.push_back(kStageThreshold);
    stages.push_back(kStateResult);
    
    registerOption("View", "", &m_currentView, stages, 3);
    
    // Bilateral filter options:
    registerOption("Color threshold", "Bilateral filter", &m_bilateralColorThreshold, 1, 255);
    registerOption("Sigma threshold", "Bilateral filter", &m_bilateralSigmaThreshold, 1, 255);
    registerOption("B",               "Bilateral filter", &m_bilateralD, 1, 15);
    
    // Adaptive threshold options:
    registerOption("Block size", "Adaptive threshold",    &m_thresholdBlockSize, 1, 9);
    registerOption("C",          "Adaptive threshold",    &m_thresholdC,       -10, 10);
}

//! Gets a sample name
std::string DrawingCanvasSample::getName() const
{
    return "Drawing canvas";
}

//! Returns a detailed sample description
std::string DrawingCanvasSample::getDescription() const
{
    return "Makes image like a blue whiteboard canvas";
}

//! Processes a frame and returns output image
bool DrawingCanvasSample::processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame)
{
    cv::cvtColor(inputFrame,
                 m_grayImage,
                 CV_BGRA2GRAY);
    
    cv::bilateralFilter(m_grayImage,
                        m_filtered,
                        m_bilateralD,
                        m_bilateralSigmaThreshold,
                        m_bilateralColorThreshold);
    
    cv::Mat grayImage3channes;
    cv::Mat a[] = { m_filtered, m_filtered, m_filtered };
    cv::merge(a, 3, grayImage3channes);
    
    cv::adaptiveThreshold(m_grayImage, bgMask, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C , CV_THRESH_BINARY, 1 + 2*m_thresholdBlockSize, m_thresholdC);
    
    cv::Mat fgMask(bgMask.size(), bgMask.type());
    fgMask = cv::Scalar(255);
    fgMask ^= bgMask;
    
    cv::Mat b[] = { bgMask, bgMask, bgMask };
    cv::merge(b, 3, bgMask3channels);
    
    cv::Mat c[] = { fgMask, fgMask, fgMask};
    cv::merge(c, 3, fgMask3channels);
    
    cv::Scalar backgroundColor = CV_RGB(56, 138, 239);
    cv::Scalar foregroundColor = CV_RGB(255,255,255);
    
    cv::multiply(grayImage3channes, backgroundColor, bgColor, 1.0f / 255.f);
    cv::multiply(grayImage3channes, foregroundColor, fgColor, 100.0f / 255.0f);
    
    if (m_currentView == kStageGrayscale)
    {
        cv::cvtColor(m_grayImage, outputFrame, CV_GRAY2BGRA);        
    }
    else if (m_currentView == kStageBilateralFilter)
    {
        cv::cvtColor(m_filtered, outputFrame, CV_GRAY2BGRA);        
    }
    else if (m_currentView == kStageThreshold)
    {
        cv::cvtColor(bgMask, outputFrame, CV_GRAY2BGRA);
    }
    else
    {
        cv::Mat resul = (bgColor & bgMask3channels) + (fgColor & fgMask3channels);
        cv::cvtColor(resul, outputFrame, CV_BGR2BGRA);
    }
    
    return true;
}
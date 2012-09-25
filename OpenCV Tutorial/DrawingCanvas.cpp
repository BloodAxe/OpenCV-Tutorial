//
//  DrawingCanvas.cpp
//  OpenCV Tutorial
//
//  Created by BloodAxe on 9/4/12.
//
//

#include "DrawingCanvas.h"

//! Gets a sample name
std::string DrawingCanvasSample::getName() const
{
    return "Drawing canvas";
}

//! Returns a detailed sample description
std::string DrawingCanvasSample::getDescription() const
{
        return "";
}

//! Processes a frame and returns output image
bool DrawingCanvasSample::processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame)
{
    cv::Mat grayImage;
    cv::cvtColor(inputFrame, grayImage, CV_BGRA2GRAY);
    //cv::equalizeHist(grayImage, grayImage);
    
    cv::Mat tmp;
    cv::bilateralFilter(grayImage, tmp, 5, 100, 100);
    grayImage = tmp;
    
    cv::Mat grayImage3channes;
    cv::Mat a[] = {  grayImage, grayImage, grayImage };
    cv::merge(a, 3, grayImage3channes);
    
    cv::Mat bgMask;
    cv::adaptiveThreshold(grayImage, bgMask, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C , CV_THRESH_BINARY, 9, 5);
    
    cv::Mat fgMask(bgMask.size(), bgMask.type());
    fgMask = cv::Scalar(255);
    fgMask ^= bgMask;
    
    cv::Mat bgMask3channels;
    cv::Mat b[] = { bgMask, bgMask, bgMask };
    cv::merge(b, 3, bgMask3channels);
    
    cv::Mat fgMask3channels;
    cv::Mat c[] = { fgMask, fgMask, fgMask};
    cv::merge(c, 3, fgMask3channels);
    
    cv::Scalar backgroundColor = CV_RGB(56, 138, 239);
    cv::Scalar foregroundColor = CV_RGB(255,255,255);
    
    cv::Mat bgColor, fgColor;
    cv::multiply(grayImage3channes, backgroundColor, bgColor, 1.0f / 255.f);
    cv::multiply(grayImage3channes, foregroundColor, fgColor, 100.0f / 255.0f);
        
    //cv::imshow("Grayscale (equalized)", grayImage);
    //cv::imshow("Background",            bgMask);
    //cv::imshow("Foreground",            fgMask);
    //cv::imshow("Colored background",    bgColor & bgMask3channels);
    //cv::imshow("Colored foreground",    fgColor & fgMask3channels);
    
    cv::Mat resul = (bgColor & bgMask3channels) + (fgColor & fgMask3channels);
    cv::cvtColor(resul, outputFrame, CV_BGR2BGRA);
}
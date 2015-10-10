//
//  CameraCalibrationSample.cpp
//  OpenCV Tutorial
//
//  Created by BloodAxe on 1/29/15.
//
//

#include "CameraCalibrationSample.h"
#include <iostream>

CameraCalibrationSample::CameraCalibrationSample()
: mAlg(cv::Size(4, 11), ACIRCLES_GRID)
, frame(0)
{
}

std::string CameraCalibrationSample::getDescription() const
{
    return  "CameraCalibrationSample";
}

std::string CameraCalibrationSample::getName() const
{
    return  "Camera Calibration";
}

bool CameraCalibrationSample::processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame)
{
    frame++;
    
    if (frame % 50)
        return false;
    
    cv::cvtColor(inputFrame, gray, cv::COLOR_BGR2GRAY);
    
    CameraCalibrationAlgorithm::VectorOf2DPoints corners2d;
    if (mAlg.detectCorners(gray, corners2d))
    {
        mCorners.push_back(corners2d);
        std::cout << "Detected" << std::endl;
    }
    
    if (mCorners.size() > 10)
    {
        cv::Mat K, D;
        mAlg.calibrateCamera(mCorners, inputFrame.size(), K, D);
        std::cout << K << std::endl;
        std::cout << D << std::endl;
        
        mAlg = CameraCalibrationAlgorithm(cv::Size(11, 4), ACIRCLES_GRID);
        mCorners.clear();
    }
    
    return true;
}

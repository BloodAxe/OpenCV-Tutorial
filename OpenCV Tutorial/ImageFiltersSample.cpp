//
//  ImageFiltersSample.cpp
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "ImageFiltersSample.h"

//! Gets a sample name
std::string ImageFiltersSample::getName() const
{
  return "Image filtering";
}

//! Returns a detailed sample description
std::string ImageFiltersSample::getDescription() const
{
  return "Demonstrate various image filtering techniques";
}

//! Processes a frame and returns output image 
bool ImageFiltersSample::processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame)
{
  cv::Mat bgr;
  cv::cvtColor(inputFrame, bgr, CV_BGRA2BGR);
  
  cv::Mat filtered;
  cv::bilateralFilter(bgr, filtered, 8, 120, 100);
  
  cv::cvtColor(filtered, outputFrame, CV_BGR2BGRA);
  
  return true;
}
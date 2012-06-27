//
//  ContourDetectionSample.cpp
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "ContourDetectionSample.h"

//! Gets a sample name
std::string ContourDetectionSample::getName() const
{
  return "Contour detection";
}

//! Returns a detailed sample description
std::string ContourDetectionSample::getDescription() const
{
  return "Image contour detection is fundamental to many image analysis applications, including image segmentation, object recognition and classiﬁcation.";
}

std::string ContourDetectionSample::getSampleIcon() const
{
  return "ContourDetectionSampleIcon.png";
}

//! Returns true if this sample requires setting a reference image for latter use
bool ContourDetectionSample::isReferenceFrameRequired() const
{
  return false;
}

//! Sets the reference frame for latter processing
void ContourDetectionSample::setReferenceFrame(const cv::Mat& reference)
{
  
}

//! Processes a frame and returns output image 
bool ContourDetectionSample::processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame)
{
  
  return true;
  //cv::findC
}
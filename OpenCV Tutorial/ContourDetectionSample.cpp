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

//! Processes a frame and returns output image 
bool ContourDetectionSample::processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame)
{
  getGray(inputFrame, gray);
 
  cv::Mat edges;
  cv::Canny(gray, edges, 50, 150);

  std::vector< std::vector<cv::Point> > c;
  
	cv::findContours(edges, c, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

  inputFrame.copyTo(outputFrame);
  cv::drawContours(outputFrame, c, -1, CV_RGB(0,200,0));
  
  return true;
}
//
//  CartoonFilter.cpp
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/19/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "CartoonFilter.h"

CartoonFilter::CartoonFilter()
  : sp(15)
  , sr(40)
{
  registerOption("Spatial window radius", "", &sp, 1, 50);
  registerOption("Color window radius", "",   &sr, 1, 50);
}

std::string CartoonFilter::getName() const
{
  return "Cartoon filter";
}

std::string CartoonFilter::getDescription() const
{
  return "This sample performs cartoon-like effect";
}

bool CartoonFilter::processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame)
{
  // Pseudocode
  // 1. Run Canny or Sobel + Threshold to get binary edges
  // 1.1 Dilate edge image to "grow" edges
  // 2. Perform watershed segmentation to find connected regions
  // 3. Reduce the color by finding average color for each region and assigning it to whole region
  // 4. Or use http://stackoverflow.com/questions/3954484/cartoonizing-real-images technique
  
  cv::cvtColor(inputFrame, bgr, CV_BGRA2BGR);
  cv::pyrMeanShiftFiltering(bgr.clone(), bgr, sp, sr);
  getGray(bgr, gray);
  cv::Canny(gray, edges, 150, 150);
  
  cv::cvtColor(edges, edgesBgr, CV_GRAY2BGR);
  
  bgr = bgr - edgesBgr;
  
  cv::cvtColor(bgr, outputFrame, CV_BGR2BGRA);
  return true;
}
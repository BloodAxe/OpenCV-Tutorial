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
  cv::cvtColor(inputFrame, bgr, CV_BGRA2BGR);
  cv::pyrMeanShiftFiltering(bgr.clone(), bgr, sp, sr);

  getGray(bgr, gray);
  cv::Canny(gray, edges, 150, 150);
  
  cv::cvtColor(edges, edgesBgr, CV_GRAY2BGR);
  
  bgr = bgr - edgesBgr;
  
  cv::cvtColor(bgr, outputFrame, CV_BGR2BGRA);
  return true;
}
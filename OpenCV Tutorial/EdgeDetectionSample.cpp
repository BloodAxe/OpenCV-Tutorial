//
//  EdgeDetectionSample.cpp
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/24/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "EdgeDetectionSample.h"

EdgeDetectionSample::EdgeDetectionSample()
: m_showOnlyEdges(true)
, m_algorithmName("Canny")
, m_cannyLoThreshold(250)
, m_cannyHiThreshold(250)
, m_cannyAperture(1)
, m_harrisBlockSize(2)
, m_harrisapertureSize(3)
, m_harrisK(0.04f)
, m_harrisThreshold(200)
{
  
  std::vector<std::string> algos;
  algos.push_back("Canny");
  algos.push_back("Sobel");
  algos.push_back("Schaar");
  //algos.push_back("Harris"); Harris detector temporary disabled
  
  registerOption("Algorithm",       "", &m_algorithmName, algos,1);
  registerOption("Show only edges", "", &m_showOnlyEdges);
  
  // Canny detector options
  registerOption("Threshold 1", "Canny", &m_cannyLoThreshold, 0, 256);
  registerOption("Threshold 2", "Canny", &m_cannyHiThreshold, 0, 256);
  registerOption("Aperture",    "Canny", &m_cannyAperture, 1, 3);
    
  // Sobel detector options
  
  // Harris detector options
  //registerOption("Block size",    "Harris", &m_harrisBlockSize, 1, 5);
  //registerOption("Aperture size", "Harris", &m_harrisapertureSize, 1,5);
  //registerOption("K",             "Harris", &m_harrisK, 0, 1);
  //registerOption("Threshold",     "Harris", &m_harrisThreshold, 0, 255);
}

//! Gets a sample name
std::string EdgeDetectionSample::getName() const
{
  return "Edge detection";
}

/*
std::string EdgeDetectionSample::getSampleIcon() const
{
  return "EdgeDetectionSampleIcon.png";
}
*/

//! Returns a detailed sample description
std::string EdgeDetectionSample::getDescription() const
{
  return "Edge detection is a fundamental tool in image processing, machine vision and computer vision, particularly in the areas of feature detection and feature extraction, which aim at identifying points in a digital image at which the image brightness changes sharply or, more formally, has discontinuities. ";
}

//! Processes a frame and returns output image 
bool EdgeDetectionSample::processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame)
{
  getGray(inputFrame, grayImage);
  
  if (m_algorithmName == "Canny")
  {
    cv::Canny(grayImage, edges, m_cannyLoThreshold, m_cannyHiThreshold, m_cannyAperture * 2 + 1);
  }
  else if (m_algorithmName == "Sobel")
  {
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;

    /// Gradient X
    cv::Sobel( grayImage, grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT );   
    cv::convertScaleAbs( grad_x, abs_grad_x );
    
    /// Gradient Y  
    cv::Sobel( grayImage, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT );   
    cv::convertScaleAbs( grad_y, abs_grad_y );
    
    /// Total Gradient (approximate)
    cv::addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, edges );
  }
  else if (m_algorithmName == "Schaar")
  {
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
        
    /// Gradient X
    cv::Scharr( grayImage, grad_x, ddepth, 1, 0, scale, delta, cv::BORDER_DEFAULT );
    cv::convertScaleAbs( grad_x, abs_grad_x );
    
    /// Gradient Y  
    cv::Scharr( grayImage, grad_y, ddepth, 0, 1, scale, delta, cv::BORDER_DEFAULT );
    cv::convertScaleAbs( grad_y, abs_grad_y );
    
    /// Total Gradient (approximate)
    cv::addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, edges );
  }
  else if (m_algorithmName == "Harris")
  {        
    /// Detecting corners
    cv::cornerHarris( grayImage, dst, m_harrisBlockSize, m_harrisapertureSize, m_harrisK, cv::BORDER_DEFAULT );
    
    /// Normalizing
    cv::normalize( dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat() );
    cv::convertScaleAbs( dst_norm, dst_norm_scaled ); 
    
    //edges = dst_norm_scaled;
    /// Drawing a circle around corners
    cv::threshold(dst_norm_scaled, edges, m_harrisThreshold, 255, CV_THRESH_BINARY);
    /*
    for( int j = 0; j < dst_norm.rows ; j++ )
    { 
      for( int i = 0; i < dst_norm.cols; i++ )
      {
        if( (int) dst_norm.at<float>(j,i) > m_harrisThreshold )
        {   
          circle( dst_norm_scaled, cv::Point( i, j ), 5,  cv::Scalar(0), 2, 8, 0 ); 
        }
      } 
    }*/
    
    //edges = dst_norm_scaled;
    //cv::cvtColor(dst_norm_scaled, outputFrame, CV_GRAY2BGRA);
  }
  else
  {
    std::cerr << "Unsupported algorithm:" << m_algorithmName << std::endl;
    assert(false);
  }
  
  if (m_showOnlyEdges)
  {
    cv::cvtColor(edges, outputFrame, CV_GRAY2BGRA);
  }
  else 
  {
    cv::cvtColor(grayImage * 0.25 +  0.75 * edges, outputFrame, CV_GRAY2BGRA);
  }
  return true;
}
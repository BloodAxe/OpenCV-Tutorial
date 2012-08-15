//
//  ImageFiltersSample.cpp
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "ImageFiltersSample.h"
#include "cvneon.h"

ImageFiltersSample::ImageFiltersSample()
: useNeonOptimized(false)
{
  m_sepiaKernel.create(4, 4);

  m_sepiaKernel(0,0) = 0.272; m_sepiaKernel(0,1) = 0.534; m_sepiaKernel(0,2) = 0.131; m_sepiaKernel(0,3) = 0;
  m_sepiaKernel(1,0) = 0.349; m_sepiaKernel(1,1) = 0.686; m_sepiaKernel(1,2) = 0.168; m_sepiaKernel(1,3) = 0;
  m_sepiaKernel(2,0) = 0.393; m_sepiaKernel(2,1) = 0.769; m_sepiaKernel(2,2) = 0.189; m_sepiaKernel(2,3) = 0;
  m_sepiaKernel(3,0) = 0;     m_sepiaKernel(3,1) = 0;     m_sepiaKernel(3,2) = 0;     m_sepiaKernel(3,3) = 1;
  
  cv::transpose(m_sepiaKernel, m_sepiaKernelT);
  
  m_contrastKernel = cv::Mat_<float>::eye(4,4);
  m_alpha = 1;
  m_bias = 0;
  
  std::vector<std::string> effects;
  effects.push_back("Sepia");
  effects.push_back("Negative");
  effects.push_back("Adjustments");
  
  registerOption("Effect", "", &m_currentEffect, effects, 0);
  registerOption("Contrast", "Adjustmens",   &m_alpha, 0, 2);
  registerOption("Brightness", "Adjustmens", &m_bias, -128, 128);  

#if (!TARGET_IPHONE_SIMULATOR)
  registerOption("Use NEON" ,"Performance", &useNeonOptimized);
#endif
}

std::string ImageFiltersSample::getName() const
{
  return "Image filtering";
}

std::string ImageFiltersSample::getUserFriendlyName() const
{
  return getName() + " - " + m_currentEffect + " effect";
}

//! Returns a detailed sample description
std::string ImageFiltersSample::getDescription() const
{
  return "Demonstrate various image filtering techniques";
}

//! Processes a frame and returns output image 
bool ImageFiltersSample::processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame)
{  
  if (outputFrame.rows != inputFrame.rows || outputFrame.cols != inputFrame.cols || outputFrame.channels() != 4)
  {
    outputFrame.create(inputFrame.rows, inputFrame.cols, CV_8UC4);
  }
  
  if (m_currentEffect == "Sepia")
  {
    sepia(inputFrame, outputFrame);
  }
  else if (m_currentEffect == "Negative")
  {
    negative(inputFrame, outputFrame);
  }
  else if (m_currentEffect == "Adjustments")
  {
    contrastAndBrightnessAdjust(inputFrame, outputFrame);
  }
  
  return true;
}

void ImageFiltersSample::sepia(const cv::Mat& inputFrame, cv::Mat& outputFrame)
{
#if (!TARGET_IPHONE_SIMULATOR)
  if (useNeonOptimized)
  {
    cv::neon_transform_bgra(inputFrame, outputFrame, m_sepiaKernelT);
  }
  else
  {
    cv::transform(inputFrame, outputFrame, m_sepiaKernel);
  }

  //cv::Vec4b src = outputFrame.at<cv::Vec4b>(10,10);
  //std::cout << (int)src[0] << "," << (int)src[1] << "," << (int)src[2] << "," << (int)src[3] << std::endl;
#else
  cv::transform(inputFrame, outputFrame, m_sepiaKernel);
#endif
}

void ImageFiltersSample::negative(const cv::Mat& inputFrame, cv::Mat& outputFrame)
{
  cv::Vec4b min(255,255,255, 0);
  cv::Vec4b max(  0,  0,  0, 255);
    
  for (int row  = 0; row < inputFrame.rows; row++)
  {
    for (int col = 0; col < inputFrame.cols; col++)
    {
      const cv::Vec4b& src = inputFrame.at<cv::Vec4b>(row, col);

      min[0] = std::min(min[0], src[0]);
      min[1] = std::min(min[1], src[1]);
      min[2] = std::min(min[2], src[2]);
      //min[3] = std::min(min[3], src[3]);
      
      max[0] = std::max(max[0], src[0]);
      max[1] = std::max(max[1], src[1]);
      max[2] = std::max(max[2], src[2]);
      //max[3] = std::max(max[3], src[3]);
    }
  }
  
  for (int row  = 0; row < inputFrame.rows; row++)
  {
    for (int col = 0; col < inputFrame.cols; col++)
    {
      cv::Vec4b result = (max - inputFrame.at<cv::Vec4b>(row, col)) + min;
      result[3] = 0xFF;
      outputFrame.at<cv::Vec4b>(row, col) = result;
    }
  }
}

void ImageFiltersSample::contrastAndBrightnessAdjust(const cv::Mat& inputFrame, cv::Mat& outputFrame)
{
  m_contrastKernel(0,0) = m_contrastKernel(1,1) = m_contrastKernel(2,2) = m_alpha;
  
#if (!TARGET_IPHONE_SIMULATOR)
    if (useNeonOptimized)
    {
        cv::neon_transform_bgra(inputFrame, outputFrame, m_contrastKernel);
    }
    else
    {
        cv::transform(inputFrame, outputFrame, m_contrastKernel);
    }
#else
    cv::transform(inputFrame, outputFrame, m_contrastKernel);
#endif
    
  outputFrame += cv::Scalar(m_bias,m_bias, m_bias, 0);
}



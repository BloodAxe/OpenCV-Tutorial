//
//  ImageFiltersSample.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef OpenCV_Tutorial_ImageFiltersSample_h
#define OpenCV_Tutorial_ImageFiltersSample_h

#include "SampleBase.h"

class ImageFiltersSample : public SampleBase
{
public:
  ImageFiltersSample();
  
  //! Gets a sample name
  virtual std::string getName() const;
  
  virtual std::string getUserFriendlyName() const;
  
  //! Returns a detailed sample description
  virtual std::string getDescription() const;
    
  //! Processes a frame and returns output image 
  virtual bool processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame);

private:
  void sepia(const cv::Mat& inputFrame, cv::Mat& outputFrame);
  
  void negative(const cv::Mat& inputFrame, cv::Mat& outputFrame);

  void contrastAndBrightnessAdjust(const cv::Mat& inputFrame, cv::Mat& outputFrame);
  
private:
  std::string m_currentEffect;
  
  cv::Mat_<float> m_sepiaKernel;
  cv::Mat_<float> m_sepiaKernelT;
  
  float m_alpha;
  int   m_bias;
  
  cv::Mat_<float> m_contrastKernel;
  cv::Mat_<float> m_contrastKernelT;
  
  bool useNeonOptimized;
};

#endif

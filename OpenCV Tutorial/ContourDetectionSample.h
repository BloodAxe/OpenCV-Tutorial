//
//  ContourDetectionSample.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef OpenCV_Tutorial_ContourDetectionSample_h
#define OpenCV_Tutorial_ContourDetectionSample_h

#include "SampleBase.h"

class ContourDetectionSample : public SampleBase
{
  //! Gets a sample name
  virtual std::string getName() const;
  
  virtual std::string getSampleIcon() const;
  
  //! Returns a detailed sample description
  virtual std::string getDescription() const;
    
  //! Processes a frame and returns output image 
  virtual bool processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame);

private:
  cv::Mat gray, edges;
};

#endif

//
//  EdgeDetectionSample.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/24/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef OpenCV_Tutorial_EdgeDetectionSample_h
#define OpenCV_Tutorial_EdgeDetectionSample_h


#include "SampleBase.h"

class EdgeDetectionSample : public SampleBase
{
  //! Gets a sample name
  virtual std::string getName() const;
  
  virtual std::string getSampleIcon() const;
  
  //! Returns a detailed sample description
  virtual std::string getDescription() const;
    
  //! Processes a frame and returns output image 
  virtual bool processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame);
  
};

#endif

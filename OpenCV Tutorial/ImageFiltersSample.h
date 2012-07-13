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
  //! Gets a sample name
  virtual std::string getName() const;
  
  //! Returns a detailed sample description
  virtual std::string getDescription() const;
    
  //! Processes a frame and returns output image 
  virtual bool processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame);

};

#endif

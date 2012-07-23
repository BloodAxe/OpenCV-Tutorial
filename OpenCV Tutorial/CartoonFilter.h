//
//  CartoonFilter.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/19/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef OpenCV_Tutorial_CartoonFilter_h
#define OpenCV_Tutorial_CartoonFilter_h

#include "SampleBase.h"

class CartoonFilter : public SampleBase
{
public:
  CartoonFilter();
  
  //! Gets a sample name
  virtual std::string getName() const;
  
  //! Returns a detailed sample description
  virtual std::string getDescription() const;
  
  //! Processes a frame and returns output image 
  virtual bool processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame);
  
private:
  float sp;
  float sr;
  
  cv::Mat gray, edges;
  cv::Mat hsv;
  
  cv::Mat bgr, img0;
  cv::Mat edgesBgr;

};


#endif

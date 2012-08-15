//
//  VideoTracking.hpp
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef OpenCV_Tutorial_VideoTracking_hpp
#define OpenCV_Tutorial_VideoTracking_hpp

#include "SampleBase.h"

class VideoTrackingSample : public SampleBase
{
public:
  VideoTrackingSample();
  
  //! Gets a sample name
  virtual std::string getName() const;
  
  //! Returns a user-friendly name for displaying as description
  virtual std::string getUserFriendlyName() const;
  
  //! Returns a detailed sample description
  virtual std::string getDescription() const;
  
  //! Processes a frame and returns output image 
  virtual bool processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame);
  
    virtual std::string getSampleIcon() const;
private:
  int m_maxNumberOfPoints;
  
  cv::Mat m_prevImg;
  cv::Mat m_nextImg;
  cv::Mat m_mask;
  
  std::vector<cv::Point2f>  m_prevPts;
  std::vector<cv::Point2f>  m_nextPts;

  std::vector<cv::KeyPoint> m_prevKeypoints;
  std::vector<cv::KeyPoint> m_nextKeypoints;
  
  cv::Mat                   m_prevDescriptors;
  cv::Mat                   m_nextDescriptors;
  
  std::vector<unsigned char> m_status;
  std::vector<float>         m_error;
  
  cv::ORB       m_orbFeatureEngine;
  cv::BFMatcher m_orbMatcher;
  
  cv::GridAdaptedFeatureDetector m_fastDetector;
  cv::BriefDescriptorExtractor m_briefExtractor;
  cv::BFMatcher                m_briefMatcher;
  
  cv::Ptr<cv::FeatureDetector> m_detector;
  std::string m_activeTrackingAlgorithm;
};


#endif

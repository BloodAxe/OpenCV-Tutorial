//
//  FeatureDetection.h
//  OpenCV Tutorial
//
//  Created by Anton Belodedenko on 25/07/2012.
//  Copyright (c) 2012 computer-vision-talks.com. All rights reserved.
//

#ifndef OpenCV_Tutorial_FeatureDetection_h
#define OpenCV_Tutorial_FeatureDetection_h

#include "SampleBase.h"

class FeatureDetectionSample : public SampleBase
{
public:
    FeatureDetectionSample();
    
    //! Gets a sample name
    virtual std::string getName() const;
    
    virtual std::string getSampleIcon() const;
    
    //! Returns a detailed sample description
    virtual std::string getDescription() const;
        
    //! Processes a frame and returns output image 
    virtual bool processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame);
    
private:
    cv::Mat grayImage;
    
    std::vector<cv::KeyPoint> objectKeypoints;
    
    std::string m_detectorName;
    std::vector<std::string> m_alorithms;
    
    cv::Ptr<cv::ORB>  m_ORB;
    cv::Ptr<cv::AKAZE> m_AKAZE;
    cv::Ptr<cv::FastFeatureDetector> m_FAST;
    
    int m_maxFeatures;
    int m_fastThreshold;
};

#endif

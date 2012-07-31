//
//  FeatureDetection.h
//  OpenCV Tutorial
//
//  Created by Anton Belodedenko on 25/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
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
    
    //! Returns true if this sample requires setting a reference image for latter use
    virtual bool isReferenceFrameRequired() const;
    
    //! Sets the reference frame for latter processing
    virtual void setReferenceFrame(const cv::Mat& reference);
    
    // clears reference frame parameters
    virtual void resetReferenceFrame() const;
    
    //! Processes a frame and returns output image 
    virtual bool processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame);
    
private:
    cv::Mat grayImage, objectImage, objectDescriptors;
    
    std::vector<cv::KeyPoint> objectKeypoints;
    
    std::string m_fdAlgorithmName, m_feAlgorithmName;
    
    // SURF detector options:
    int m_hessianThreshold;
    
    // ORB detector options:
    int m_nFeatures;
    
    // Minimum number of features nesessary to compute homography
    int m_minMatches;
    
    // draw epipolar lines option
    bool m_drawEpipolarLines;
    
    // draw matched keyponts option
    bool m_drawMatches;
    
    // draw perspective option
    bool m_drawPerspective;
};

#endif

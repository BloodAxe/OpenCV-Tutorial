//
//  ObjectTrackingSample.h
//  OpenCV Tutorial
//
//  Created by Anton Belodedenko on 26/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef OpenCV_Tutorial_ObjectTrackingSample_h
#define OpenCV_Tutorial_ObjectTrackingSample_h

#include "SampleBase.h"

class ObjectTrackingSample : public SampleBase
{
public:
    ObjectTrackingSample();
    
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
    cv::Mat imageNext, imagePrev;
    
    std::vector<uchar> status;
    
    std::vector<float> err;
    
    std::string m_algorithmName;
    
    cv::vector<cv::Point2f> pointsPrev, pointsNext;
    
    // optical flow options
    int m_maxCorners;
};

#endif

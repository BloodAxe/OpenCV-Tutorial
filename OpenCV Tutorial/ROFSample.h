//
//  ROFSample.h
//  OpenCV Tutorial
//
//  Created by Emmanuel d'Angelo on 20.07.12.
//  Copyright (c) 2012 EPFL/STI/IEL/LTS2. All rights reserved.
//

#ifndef OpenCV_Tutorial_ROFSample_h
#define OpenCV_Tutorial_ROFSample_h

#include "SampleBase.h"

class ROFSample : public SampleBase
{
public:
    ROFSample();
    
    //! Gets a sample name
    virtual std::string getName() const;
    
    virtual std::string getSampleIcon() const;
    
    //! Returns a detailed sample description
    virtual std::string getDescription() const;
    
    //! Processes a frame and returns output image 
    virtual bool processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame);
    
private:
    cv::Mat grayImage;
    
    // Diffusion options
    float m_lambda;
    int m_iterations;
    int m_lambdaExponent;
    bool m_processColor;
};

#endif

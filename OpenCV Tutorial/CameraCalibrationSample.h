//
//  CameraCalibrationSample.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 1/29/15.
//
//

#pragma once 

#include "SampleBase.h"
#include "CameraCalibrationAlgorithm.h"

class CameraCalibrationSample : public SampleBase
{
public:
    CameraCalibrationSample();
    
    //! Gets a sample name
    virtual std::string getName() const;
    
    //! Returns a detailed sample description
    virtual std::string getDescription() const;
    
    //! Processes a frame and returns output image
    virtual bool processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame);
    
private:
    int frame;
    cv::Mat gray;
    CameraCalibrationAlgorithm mAlg;
    
    CameraCalibrationAlgorithm::VectorOfVectorOf2DPoints mCorners;
};


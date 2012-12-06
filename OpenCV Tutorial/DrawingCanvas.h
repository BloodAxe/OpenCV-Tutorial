//
//  DrawingCanvas.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 9/4/12.
//
//

#ifndef OpenCV_Tutorial_DrawingCanvas_h
#define OpenCV_Tutorial_DrawingCanvas_h

#include "SampleBase.h"

class DrawingCanvasSample : public SampleBase
{
public:
    DrawingCanvasSample();
    
    //! Gets a sample name
    virtual std::string getName() const;
    
    //! Returns a detailed sample description
    virtual std::string getDescription() const;
    
    //! Processes a frame and returns output image
    virtual bool processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame);
    
private:
    std::string m_currentView;
    
    cv::Mat m_grayImage;
    cv::Mat m_filtered;
    
    cv::Mat bgColor, fgColor;
    cv::Mat bgMask3channels;
    cv::Mat fgMask3channels;
    cv::Mat bgMask;

    float m_bilateralSigmaThreshold;
    float m_bilateralColorThreshold;
    float m_bilateralD;
    
    int m_thresholdBlockSize;
    float m_thresholdC;
};


#endif

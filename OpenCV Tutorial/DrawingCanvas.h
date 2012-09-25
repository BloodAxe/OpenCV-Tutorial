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
    //! Gets a sample name
    virtual std::string getName() const;
    
    //! Returns a detailed sample description
    virtual std::string getDescription() const;
    
    //! Processes a frame and returns output image
    virtual bool processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame);
};


#endif

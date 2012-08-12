//
//  SampleBase.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef OpenCV_Tutorial_SampleBase_h
#define OpenCV_Tutorial_SampleBase_h

#include "SampleOptions.h"

//! Base class for all samples
class SampleBase
{
public:
    //! Gets a sample name
    virtual std::string getName() const = 0;
    
    //! Returns a user-friendly name for displaying as description
    virtual std::string getUserFriendlyName() const;
    
    //! Returns a detailed sample description
    virtual std::string getDescription() const = 0;
    
    //! Returns a icon for this sample. The default implementation returns empty string
    virtual std::string getSampleIcon() const;
    
    //! Returns true if this sample requires setting a reference image for latter use
    virtual bool isReferenceFrameRequired() const;
    
    //! Sets the reference frame for latter processing
    virtual void setReferenceFrame(const cv::Mat& reference);
    
    // Resets the reference frame
    virtual void resetReferenceFrame() const;
    
    //! Processes a frame and returns output image 
    virtual bool processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame) = 0;
    
    bool hasIcon() const;
    
    typedef std::vector<SampleOption*> OptionsSection;
    typedef std::map<std::string, OptionsSection> OptionsMap;
    
    const OptionsMap& getOptions() const;
    
protected:
    void registerOption(std::string name, std::string section, bool  * value);
    void registerOption(std::string name, std::string section, int   *  value, int min, int max);
    void registerOption(std::string name, std::string section, float *  value, float min, float max);
    void registerOption(std::string name, std::string section, double *  value, double min, double max);
    void registerOption(std::string name, std::string section, std::string* value, std::vector<std::string> stringEnums, int defaultValue = 0);
    
    static void getGray(const cv::Mat& input, cv::Mat& gray);
    
private:
    OptionsMap m_optionsWithSections;
};

#endif

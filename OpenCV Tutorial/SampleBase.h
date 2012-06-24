//
//  SampleBase.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef OpenCV_Tutorial_SampleBase_h
#define OpenCV_Tutorial_SampleBase_h

typedef enum { OptionKindBoolean, OptionKindInt32, OptionKindFloat, OptionKindStringEnum } OptionKind;

struct SampleOption
{
  OptionKind kind;
  void * ptr;
  
//  virtual bool parse(std::string& value) = 0;
//  virtual std::string toString
};

//! Base class for all samples
class SampleBase
{
public:
  //! Gets a sample name
  virtual std::string getName() const = 0;
  
  //! Returns a detailed sample description
  virtual std::string getDescription() const = 0;
  
  //! Returns true if this sample requires setting a reference image for latter use
  virtual bool isReferenceFrameRequired() const = 0;
  
  //! Sets the reference frame for latter processing
  virtual void setReferenceFrame(const cv::Mat& reference) = 0;
  
  //! Processes a frame and returns output image 
  virtual bool processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame) = 0;
  
  const std::vector<SampleOption>& getOptions();
  
protected:
  void registerOption(std::string name, bool  * value);
  void registerOption(std::string name, int   *  value, int min, int max);
  void registerOption(std::string name, float *  value, float min, float max);
  
private:
  std::vector<SampleOption> m_options;
};

#endif

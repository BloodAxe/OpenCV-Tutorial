//
//  SampleBase.cpp
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "SampleBase.h"
#include <iostream>
#include "cvneon.h"

std::string SampleBase::getUserFriendlyName() const
{
  return getName();
}

std::string SampleBase::getSampleIcon() const
{
  return "";
}

const SampleBase::OptionsMap& SampleBase::getOptions() const
{
  return m_optionsWithSections;
}

void SampleBase::registerOption(std::string name, std::string section, bool  * value)
{
  SampleOption * opt = new BooleanOption(name, section, value);
  m_optionsWithSections[section].push_back(opt);
}

void SampleBase::registerOption(std::string name, std::string section, int   *  value, int min, int max)
{
  SampleOption * opt = new Int32Option(name, section, value, min, max);
  m_optionsWithSections[section].push_back(opt);
  
  *value = std::max(min, std::max(min, *value));
}

void SampleBase::registerOption(std::string name, std::string section, float *  value, float min, float max)
{
  SampleOption * opt = new FloatOption(name, section, value, min, max);
  m_optionsWithSections[section].push_back(opt);

  *value = std::max(min, std::max(min, *value));
}

void SampleBase::registerOption(std::string name, std::string section, double *  value, double min, double max)
{
  SampleOption * opt = new DoubleOption(name, section, value, min, max);
  m_optionsWithSections[section].push_back(opt);

  *value = std::max(min, std::max(min, *value));
}

void SampleBase::registerOption(std::string name, std::string section, std::string* value, std::vector<std::string> stringEnums, int defaultValue)
{
  SampleOption * opt = new StringEnumOption(name, section, value, stringEnums, defaultValue);
  m_optionsWithSections[section].push_back(opt);  
  
  *value = stringEnums[defaultValue]; // Assign default value just in case
}


bool SampleBase::hasIcon() const
{
  return false == getSampleIcon().empty();
}



void SampleBase::getGray(const cv::Mat& input, cv::Mat& gray)
{
  const int numChannes = input.channels();
  
  if (numChannes == 4)
  {
#if TARGET_IPHONE_SIMULATOR
    cv::cvtColor(input, gray, CV_BGRA2GRAY);    
#else
    cv::neon_cvtColorBGRA2GRAY(input, gray);
#endif
    
  }
  else if (numChannes == 3)
  {
    cv::cvtColor(input, gray, CV_BGR2GRAY);
  }
  else if (numChannes == 1)
  {
    gray = input;
  }
}

//! Returns true if this sample requires setting a reference image for latter use
bool SampleBase::isReferenceFrameRequired() const
{
  return false;
}

//! Sets the reference frame for latter processing
void SampleBase::setReferenceFrame(const cv::Mat& reference)
{
  // Does nothing. Override this method if you need to
}

// Resets the reference frame
void SampleBase::resetReferenceFrame() const
{
    // Does nothing. Override this method if you need to
}

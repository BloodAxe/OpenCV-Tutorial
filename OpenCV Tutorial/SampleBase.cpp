//
//  SampleBase.cpp
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "SampleBase.h"
#include <iostream>

std::string SampleBase::getSampleIcon() const
{
  return "";
}

const std::vector<SampleOption>& SampleBase::getOptions()
{
  return m_options;
}

void SampleBase::registerOption(std::string name, bool  * value)
{
  SampleOption opt;

  opt.kind = OptionKindBoolean;
  opt.ptr  = value;
  
  m_options.push_back(opt);
}

void SampleBase::registerOption(std::string name, int   *  value, int min, int max)
{
  SampleOption opt;
  
  opt.kind = OptionKindBoolean;
  opt.ptr  = value;
  
  m_options.push_back(opt);
}

void SampleBase::registerOption(std::string name, float *  value, float min, float max)
{
  SampleOption opt;
  
  opt.kind = OptionKindBoolean;
  opt.ptr  = value;
  
  m_options.push_back(opt);
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
    cv::cvtColor(input, gray, CV_BGRA2GRAY);
  }
  else if (numChannes == 2)
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

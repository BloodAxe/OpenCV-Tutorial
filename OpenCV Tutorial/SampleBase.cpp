//
//  SampleBase.cpp
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "SampleBase.h"
#include "TargetConditionals.h"
#include <iostream>

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
}

void SampleBase::registerOption(std::string name, std::string section, float *  value, float min, float max)
{
  SampleOption * opt = new FloatOption(name, section, value, min, max);
  m_optionsWithSections[section].push_back(opt);
}

void SampleBase::registerOption(std::string name, std::string section, std::string* value, std::vector<std::string> stringEnums, int defaultValue)
{
  SampleOption * opt = new StringEnumOption(name, section, value, stringEnums, defaultValue);
  m_optionsWithSections[section].push_back(opt);  
}


bool SampleBase::hasIcon() const
{
  return false == getSampleIcon().empty();
}

#if (!TARGET_IPHONE_SIMULATOR)
namespace neon
{
  static void neon_asm_convert(uint8_t * __restrict dest, uint8_t * __restrict src, int numPixels)
  {
    __asm__ volatile("lsr          %2, %2, #3      \n"
                     "# build the three constants: \n"
                     "mov         r4, #28          \n" // Blue channel multiplier
                     "mov         r5, #151         \n" // Green channel multiplier
                     "mov         r6, #77          \n" // Red channel multiplier
                     "vdup.8      d4, r4           \n"
                     "vdup.8      d5, r5           \n"
                     "vdup.8      d6, r6           \n"
                     "0:						   \n"
                     "# load 8 pixels:             \n"
                     "vld4.8      {d0-d3}, [%1]!   \n"
                     "# do the weight average:     \n"
                     "vmull.u8    q7, d0, d4       \n"
                     "vmlal.u8    q7, d1, d5       \n"
                     "vmlal.u8    q7, d2, d6       \n"
                     "# shift and store:           \n"
                     "vshrn.u16   d7, q7, #8       \n" // Divide q3 by 256 and store in the d7
                     "vst1.8      {d7}, [%0]!      \n"
                     "subs        %2, %2, #1       \n" // Decrement iteration count
                     "bne         0b            \n" // Repeat unil iteration count is not zero
                     :
                     : "r"(dest), "r"(src), "r"(numPixels)
                     : "r4", "r5", "r6"
                     );
  }
  
  static void cvtBgra2Gray(const cv::Mat& input, cv::Mat& gray)
  {
    if (gray.rows != input.rows || gray.cols != input.cols || gray.type() != CV_8UC1)
    {
      gray.create(input.rows, input.cols, CV_8UC1);
    }
    
    uint8_t * __restrict dest = gray.data;
    uint8_t * __restrict src  = input.data;
    int numPixels = input.rows * input.cols;
    
    neon_asm_convert(dest, src, numPixels);
  }
}
#endif

void SampleBase::getGray(const cv::Mat& input, cv::Mat& gray)
{
  const int numChannes = input.channels();
  
  if (numChannes == 4)
  {
#if TARGET_IPHONE_SIMULATOR
    cv::cvtColor(input, gray, CV_BGRA2GRAY);    
#else
    neon::cvtBgra2Gray(input, gray);
#endif
    
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

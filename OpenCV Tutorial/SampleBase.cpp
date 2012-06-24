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
//
//  SampleOptions.cpp
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/9/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "SampleOptions.h"

SampleOption::SampleOption(const std::string& name, const std::string& section)
: m_name(name)
, m_section(section)
{
  
}



BooleanOption::BooleanOption(const std::string& name, const std::string& section, bool* value)
: SampleOption(name, section)
, m_value(value)
, m_default(*value)
{
  
}

OptionKind BooleanOption::getKind()
{
  return OptionKindBoolean;
}


bool BooleanOption::getValue() const
{
  return *m_value;
}

bool BooleanOption::setValue(bool value)
{
  bool changed = *m_value != value;
  *m_value = value;
  
  return changed;
}


Int32Option::Int32Option(const std::string& name, const std::string& section, int* value, int min, int max)
: SampleOption(name, section)
, m_value(value)
, m_min(min)
, m_max(max)
, m_default(*value)
{
  
}

OptionKind Int32Option::getKind()
{
  return OptionKindInt32;
}


int Int32Option::getValue() const { return *m_value; }

bool Int32Option::setValue(int v) 
{
  v = std::max(m_min, std::min(v, m_max));
  bool changed = *m_value != v;
  *m_value = v;
  
  return changed;
}

int Int32Option::getMaxValue() const { return m_max; }
int Int32Option::getMinValue() const { return m_min; }


FloatOption::FloatOption(const std::string& name, const std::string& section, float* value, float min, float max)
: SampleOption(name, section)
, m_value(value)
, m_min(min)
, m_max(max)
, m_default(*value)
{
  
}

OptionKind FloatOption::getKind()
{
  return OptionKindFloat;
}

float FloatOption::getValue() const 
{
  return *m_value; 
}

bool FloatOption::setValue(float v) 
{
  v = std::max(m_min, std::min(v, m_max));
  bool changed = *m_value != v;
  *m_value = v; 
  return changed;
}

float FloatOption::getMaxValue() const { return m_max; }
float FloatOption::getMinValue() const { return m_min; }

DoubleOption::DoubleOption(const std::string& name, const std::string& section, double* value, double min, double max)
: SampleOption(name, section)
, m_value(value)
, m_min(min)
, m_max(max)
, m_default(*value)
{
  
}

OptionKind DoubleOption::getKind()
{
  return OptionKindDouble;
}

double DoubleOption::getValue() const 
{
  return *m_value; 
}

bool DoubleOption::setValue(double v) 
{
  v = std::max(m_min, std::min(v, m_max));
  bool changed = *m_value != v;
  *m_value = v; 
  return changed;
}

double DoubleOption::getMaxValue() const { return m_max; }
double DoubleOption::getMinValue() const { return m_min; }


StringEnumOption::StringEnumOption(const std::string& name, const std::string& section, std::string* value, std::vector<std::string> stringEnums, int defaultValue)
: SampleOption(name, section)
, m_value(value)
, m_stringEnums(stringEnums)
, m_defaultValue(defaultValue)
{
  std::vector<std::string>::iterator i = std::find(stringEnums.begin(), stringEnums.end(), *value);
  if ( stringEnums.end() != i)
  {
    m_index = i - stringEnums.begin();
  }
  else
  {
    m_index = defaultValue;
  }
  
  *m_value = getValue();
}


OptionKind StringEnumOption::getKind()
{
  return OptionKindStringEnum;
}

int StringEnumOption::getValueIndex() const
{
  return m_index; 
}

std::string StringEnumOption::getValue() const 
{
  return m_stringEnums[m_index]; 
}

bool StringEnumOption::setValue(int newIndex)
{
  bool changed = newIndex != m_index;
  
  m_index = newIndex;
  *m_value = getValue();
  
  return changed;
}

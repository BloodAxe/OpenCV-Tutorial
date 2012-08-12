//
//  SampleOptions.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/9/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef OpenCV_Tutorial_SampleOptions_h
#define OpenCV_Tutorial_SampleOptions_h

#include <string>

typedef enum { OptionKindBoolean, OptionKindInt32, OptionKindFloat, OptionKindDouble, OptionKindStringEnum } OptionKind;

class SampleOption
{
public:
  const std::string getName() const { return m_name; }
  
  virtual OptionKind getKind() = 0;

protected:
  SampleOption(const std::string& name, const std::string& section);
  
  std::string m_name;
  std::string m_section;
};

class Int32Option : public SampleOption
{
public:
  Int32Option(const std::string& name, const std::string& section, int* value, int min, int max);
  
  virtual OptionKind getKind();

  int getValue() const; 
  bool setValue(int v); 
  
  int getMaxValue() const;
  int getMinValue() const;
  
private:
  int * m_value;
  int   m_min;
  int   m_max;
  int   m_default;
};

class FloatOption : public SampleOption
{
public:
  FloatOption(const std::string& name, const std::string& section, float* value, float min, float max);
  
  virtual OptionKind getKind();
  
  float getValue() const; 
  bool setValue(float v); 
  
  float getMaxValue() const;
  float getMinValue() const; 
  
private:
  float * m_value;
  float   m_min;
  float   m_max;  
  float   m_default;
};

class DoubleOption : public SampleOption
{
public:
  DoubleOption(const std::string& name, const std::string& section, double* value, double min, double max);
  
  virtual OptionKind getKind();
  
  double getValue() const; 
  bool setValue(double v); 
  
  double getMaxValue() const;
  double getMinValue() const; 
  
private:
  double * m_value;
  double   m_min;
  double   m_max;  
  double   m_default;
};

class BooleanOption : public SampleOption
{
public:
  BooleanOption(const std::string& name, const std::string& section, bool* value);
  
  virtual OptionKind getKind();
  
  bool getValue() const;
  bool setValue(bool value);
  
private:
  bool * m_value;
  bool   m_default;
};

class StringEnumOption : public SampleOption
{
public:
  StringEnumOption(const std::string& name, 
                   const std::string& section, 
                   std::string* value, 
                   std::vector<std::string> stringEnums,
                   int defaultValue = 0);
  
  virtual OptionKind getKind();
  
  int getValueIndex() const;
  std::string getValue() const;
  bool setValue(int newIndex);
  
  const std::vector<std::string>& getEnums() const { return m_stringEnums; }
  
private:
  int m_index;
  
  std::string* m_value;
  std::vector<std::string> m_stringEnums;
  int m_defaultValue;
  
};


#endif

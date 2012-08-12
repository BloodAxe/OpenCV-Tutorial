//
//  DoubleTableViewCell.m
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/30/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "DoubleTableViewCell.h"
#import "NSString+StdString.h"

@implementation DoubleTableViewCell

@synthesize label;
@synthesize currentValue;
@synthesize sliderValue;
@synthesize option;

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
  self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
  if (self) {
    // Initialization code
  }
  return self;
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
  [super setSelected:selected animated:animated];
  
  // Configure the view for the selected state
}

- (void) configureWithOption: (DoubleOption*) opt
{
  self.option = opt;
  
  label.text = [NSString stringWithStdString: option->getName()];
  
  sliderValue.maximumValue = self.option->getMaxValue();
  sliderValue.minimumValue = self.option->getMinValue();
  sliderValue.value =        self.option->getValue();
  
  [self sliderValueChanged:self];
}

- (IBAction)sliderValueChanged:(id)sender 
{
  currentValue.text = [NSString stringWithFormat:@"%f", sliderValue.value];
  
  bool changed = option->setValue(sliderValue.value);
  if (changed && self.delegate)
  {
    [self.delegate optionDidChanged:option];
  }
  
}
@end

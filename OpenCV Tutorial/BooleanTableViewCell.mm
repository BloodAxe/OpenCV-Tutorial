//
//  RangeValueTableViewCell.m
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "BooleanTableViewCell.h"
#import "NSString+StdString.h"

@implementation BooleanTableViewCell
@synthesize option;
@synthesize label;
@synthesize switchValue;

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self)
    {
      NSLog(@"Cell height at startup:%f", self.bounds.size.height);
        // Initialization code
    }
    return self;
}


- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

- (void) configureWithOption: (BooleanOption*) opt
{
  self.option = opt;
  
  NSLog(@"BooleanTableViewCell::configureWithOption - %d", (int)opt->getValue());

  self.label.text = [NSString stringWithStdString:opt->getName()];
  self.switchValue.on = opt->getValue();
  
}

- (IBAction)switchValueChanged:(id)sender 
{
  NSLog(@"Switch value changed to %d", (int)(self.switchValue.on));
  bool changed = option->setValue(self.switchValue.on);
  
  if (changed && self.delegate)
  {
    [self.delegate optionDidChanged:option];
  }
}

@end

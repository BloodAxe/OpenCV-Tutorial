//
//  EnumTableViewCell.m
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/9/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "EnumTableViewCell.h"
#import "NSString+StdString.h"

@implementation EnumTableViewCell
@synthesize label;
@synthesize segmentedControl;
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



- (void) configureWithOption: (StringEnumOption*) opt
{
  self.option = opt;
  self.label.text = [NSString stringWithStdString:self.option->getName()];  

  [self.segmentedControl removeAllSegments];
  
  int numberOfItems = self.option->getEnums().size();

  for (size_t i=0; i < numberOfItems; i++)
  {
    NSString * title = [NSString stringWithStdString:self.option->getEnums()[i]];
    [self.segmentedControl insertSegmentWithTitle:title atIndex:i animated:NO];
  }
  
  [self.segmentedControl setSelectedSegmentIndex:self.option->getValueIndex()];
}

- (IBAction)segmentedControlChanged:(id)sender
{
  bool changed = self.option->setValue(self.segmentedControl.selectedSegmentIndex);
  
  if (changed && self.delegate)
  {
    [self.delegate optionDidChanged:option];
  }

}
@end

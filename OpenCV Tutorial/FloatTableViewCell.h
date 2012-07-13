//
//  FloatTableViewCell.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/9/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SampleOptions.h"
#import "OptionCell.h"

@interface FloatTableViewCell : OptionCell

@property (weak, nonatomic) IBOutlet UILabel *label;
@property (weak, nonatomic) IBOutlet UILabel *currentValue;
@property (weak, nonatomic) IBOutlet UISlider *sliderValue;

@property FloatOption * option;

- (IBAction)sliderValueChanged:(id)sender;

- (void) configureWithOption: (FloatOption*) option;


@end

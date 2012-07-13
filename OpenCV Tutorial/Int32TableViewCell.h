//
//  Int32TableViewCell.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/9/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SampleOptions.h"
#import "OptionCell.h"

@interface Int32TableViewCell : OptionCell

@property (weak, nonatomic) IBOutlet UILabel *label;
@property (weak, nonatomic) IBOutlet UILabel *currentValue;
@property (weak, nonatomic) IBOutlet UISlider *sliderValue;

@property Int32Option * option;

- (IBAction)sliderValueChanged:(id)sender;

- (void) configureWithOption: (Int32Option*) option;

@end

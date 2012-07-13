//
//  RangeValueTableViewCell.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SampleOptions.h"
#import "OptionCell.h"

@interface BooleanTableViewCell : OptionCell

- (void) configureWithOption: (BooleanOption*) option;
- (IBAction)switchValueChanged:(id)sender;

@property BooleanOption * option;
@property (weak, nonatomic) IBOutlet UILabel *label;
@property (weak, nonatomic) IBOutlet UISwitch *switchValue;


@end

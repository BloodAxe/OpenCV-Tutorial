//
//  EnumTableViewCell.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/9/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SampleOptions.h"
#import "OptionCell.h"

@interface EnumTableViewCell : OptionCell

@property StringEnumOption * option;

- (void) configureWithOption: (StringEnumOption*) option;

@property (weak, nonatomic) IBOutlet UILabel *label;
@property (weak, nonatomic) IBOutlet UISegmentedControl *segmentedControl;

- (IBAction)segmentedControlChanged:(id)sender;

@end

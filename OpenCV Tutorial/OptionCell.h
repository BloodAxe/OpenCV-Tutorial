//
//  OptionCell.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SampleOptions.h"

@protocol OptionCellDelegate <NSObject>

- (void) optionDidChanged:(SampleOption*) option;

@end 


@interface OptionCell : UITableViewCell

@property (readonly) float cellHeight;
@property id<OptionCellDelegate> delegate;

@end

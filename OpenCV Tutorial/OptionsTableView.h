//
//  OptionsTableView.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/11/12.
//  Copyright (c) 2012 computer-vision-talks.com. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SampleFacade.h"
#import "OptionCell.h"

@interface OptionsTableView : UITableView

- (id) initWithFrame:(CGRect)frame 
               style:(UITableViewStyle)style 
              sample:(SampleFacade*) sample
notificationsDelegate:(id<OptionCellDelegate>) delegate;

@end

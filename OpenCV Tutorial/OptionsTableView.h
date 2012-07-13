//
//  OptionsTableView.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SampleBase.h"
#import "OptionCell.h"

@interface OptionsTableView : UITableView

- (id) initWithFrame:(CGRect)frame 
               style:(UITableViewStyle)style 
              sample:(SampleBase*) sample 
notificationsDelegate:(id<OptionCellDelegate>) delegate;

@end

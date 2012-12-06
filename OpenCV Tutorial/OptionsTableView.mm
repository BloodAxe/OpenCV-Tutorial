//
//  OptionsTableView.m
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "OptionsTableView.h"
#import "SampleOptionsTableViewDelegate.h"

@interface OptionsTableView ()
@property (nonatomic, strong) SampleOptionsTableViewDelegate * optionsFacade;
@end

@implementation OptionsTableView
@synthesize optionsFacade;

- (id) initWithFrame:(CGRect)frame 
               style:(UITableViewStyle)style 
              sample:(SampleFacade*) sample
notificationsDelegate:(id<OptionCellDelegate>) delegate
{
  if (self = [super initWithFrame:frame style:style])
  {
    [self registerNib:[UINib nibWithNibName:@"BooleanTableViewCell" bundle:[NSBundle mainBundle]] forCellReuseIdentifier:@"BooleanTableViewCell"];
    [self registerNib:[UINib nibWithNibName:@"Int32TableViewCell" bundle:[NSBundle mainBundle]] forCellReuseIdentifier:@"Int32TableViewCell"];
    [self registerNib:[UINib nibWithNibName:@"FloatTableViewCell" bundle:[NSBundle mainBundle]] forCellReuseIdentifier:@"FloatTableViewCell"];
    [self registerNib:[UINib nibWithNibName:@"EnumTableViewCell" bundle:[NSBundle mainBundle]] forCellReuseIdentifier:@"EnumTableViewCell"];
    [self registerNib:[UINib nibWithNibName:@"DoubleTableViewCell" bundle:[NSBundle mainBundle]] forCellReuseIdentifier:@"DoubleTableViewCell"];

    [self setAutoresizingMask: (UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight) ];
    self.optionsFacade = [[SampleOptionsTableViewDelegate alloc] initWithSample:sample notificationsDelegate:delegate];
    self.delegate = self.optionsFacade;
    self.dataSource = self.optionsFacade;
  }
  
  return self;
}

@end

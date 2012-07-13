//
//  OptionCell.m
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "OptionCell.h"

@implementation OptionCell
@synthesize delegate;
@synthesize cellHeight = _cellHeight;

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

- (void) awakeFromNib
{
  [super awakeFromNib];
  _cellHeight = self.bounds.size.height;
}


@end

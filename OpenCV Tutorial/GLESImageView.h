//
//  GLESView.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface GLESImageView : UIView

- (void)drawFrame:(const cv::Mat&) bgraFrame;


@end

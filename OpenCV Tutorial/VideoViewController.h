//
//  VideoViewController.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "GLESImageView.h"
#import "VideoSource.h"
#import "SampleBase.h"

@interface VideoViewController : UIViewController<VideoSourceDelegate>

@property (weak, nonatomic) IBOutlet GLESImageView *imageView;
@property (weak, nonatomic) IBOutlet UIButton *toggleCameraButton;

- (void) setSample:(SampleBase*) sample;
- (IBAction)toggleCameraPressed:(id)sender;

@end


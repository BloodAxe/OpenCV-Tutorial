//
//  BaseSampleViewController.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "SampleBase.h"

typedef void (^TweetImageCompletionHandler)(); 
typedef void (^SaveImageCompletionHandler)(); 

#define kSaveImageActionTitle  @"Save image"
#define kComposeTweetWithImage @"Tweet image"

@interface BaseSampleViewController : UIViewController

@property (readonly) SampleBase * currentSample;

- (void) configureView;
- (void) setSample:(SampleBase*) sample;
- (void) tweetImage:(UIImage*) image withCompletionHandler: (TweetImageCompletionHandler) handler;
- (void) saveImage:(UIImage*) image  withCompletionHandler: (SaveImageCompletionHandler) handler;

@end

//
//  AppDelegate.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/23/12.
//  Copyright (c) 2012 computer-vision-talks.com. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SampleBase.h"
#import "SampleFacade.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate>
{
@public
  std::vector<SampleFacade*> allSamples;
}
@property (strong, nonatomic) UIWindow *window;

@end

//
//  SampleFacade.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 8/14/12.
//
//

#import <Foundation/Foundation.h>
#import "SampleBase.h"

@interface SampleFacade : NSObject

- (id) initWithSample:(SampleBase*) sample;

@property (readonly) SampleBase * sample;

- (NSString *) title;
- (NSString *) description;
- (NSString *) friendlyName;

- (UIImage*)   smallIcon;
- (UIImage*)   largeIcon;

- (bool) processFrame:(const cv::Mat&) inputFrame into:(cv::Mat&) outputFrame;
- (UIImage*) processFrame:(UIImage*) source;

@end

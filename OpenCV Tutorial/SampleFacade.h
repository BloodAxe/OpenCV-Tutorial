//
//  SampleFacade.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 8/14/12.
//
//

#import <Foundation/Foundation.h>
#import "SampleBase.h"

typedef std::vector<SampleOption*> OptionsSection;
typedef std::map<std::string, OptionsSection> OptionsMap;

@interface SampleFacade : NSObject

- (id) initWithSample:(SampleBase*) sample;

//@property (readonly) SampleBase * sample;

- (NSString *) title;
- (NSString *) description;
- (NSString *) friendlyName;

- (UIImage*)   smallIcon;
- (UIImage*)   largeIcon;

- (bool) processFrame:(const cv::Mat&) inputFrame into:(cv::Mat&) outputFrame;

- (UIImage*) processFrame:(UIImage*) source;

- (OptionsMap) getOptions;

@property (getter = getIsReferenceFrameRequired, readonly) bool isReferenceFrameRequired;

- (void) setReferenceFrame:(cv::Mat&) referenceFrame;
- (void) resetReferenceFrame;

@end

//
//  UIImage_UIImage2OpenCV.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/25/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//


// This interface extension allows convert UIImage to cv::Mat representation and
// vice versa using full data copy in both directions.
@interface UIImage (OpenCV)

-(cv::Mat) toMat;

+(UIImage*) imageWithMat:(const cv::Mat&) image andImageOrientation: (UIImageOrientation) orientation;
+(UIImage*) imageWithMat:(const cv::Mat&) image andDeviceOrientation: (UIDeviceOrientation) orientation;

- (UIImage *)thumbnailWithSize:(int)thumbSize;

@end

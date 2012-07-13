//
//  VideoSource.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreMedia/CoreMedia.h>
#import <AVFoundation/AVFoundation.h>

@protocol VideoSourceDelegate <NSObject>

- (void) frameCaptured:(cv::Mat) frame;

@end

@interface VideoSource : NSObject<AVCaptureVideoDataOutputSampleBufferDelegate>

@property id<VideoSourceDelegate> delegate;

- (AVCaptureVideoOrientation) videoOrientation;
- (bool) hasMultipleCameras;
- (void) toggleCamera;

- (void) startRunning;
- (void) stopRunning;

@end

//! A "fake" video source that does not perform video capture but generates a simple image.
//! It's interface is equal to real video source to let us test video-processing on iOS simulator too.
@interface DummyVideoSource : NSObject

@property id<VideoSourceDelegate> delegate;

- (id) initWithFrameSize:(CGSize) frameSize;

- (AVCaptureVideoOrientation) videoOrientation;
- (bool) hasMultipleCameras;
- (void) toggleCamera;

- (void) startRunning;
- (void) stopRunning;

@end
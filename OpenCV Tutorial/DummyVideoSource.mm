//
//  VideoSource.m
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "VideoSource.h"

@interface DummyVideoSource ()
{
  int     m_frameWidth;
  int     m_frameHeight;
  cv::Mat m_frame;
  
  float xstart;
  float ystart;
  float xscale;
  float yscale;
}
@property (strong, nonatomic)   NSTimer * timer;

@end

@implementation DummyVideoSource
@synthesize delegate;
@synthesize timer;

- (id) initWithFrameSize:(CGSize) frameSize;
{
  if (self = [super init])
  {
    m_frameWidth = frameSize.width;
    m_frameHeight = frameSize.height;
    m_frame.create(m_frameHeight, m_frameWidth, CV_8UC4);

  }
  
  return self;
}

cv::Mat_<cv::Vec4b> rotate(cv::Mat_<cv::Vec4b> in)
{
  cv::Mat out;
  in.copyTo(out);
  for (int i=0;i<in.rows;i++)
  {
    for (int j=0;j<in.cols;j++)
    {
      out.at<cv::Vec4b>(i,j)=in.at<cv::Vec4b>(in.cols-j-1,i);
    }
  }
  return out;
}

-(void) timerTick:(id) sender
{
  if (delegate)
  {
    cv::rectangle(m_frame, cv::Point(0,0), cv::Point(m_frameWidth, m_frameHeight), CV_RGB(0,0,0), CV_FILLED);
    
    for (int i=0; i<10; i++)
    {
      cv::circle(m_frame, 
                 cv::Point(rand() % m_frameWidth , rand() % m_frameHeight), 
                 rand() % (std::min(m_frameHeight, m_frameWidth) / 5), 
                 CV_RGB(rand() % 256, rand() % 256, rand() % 256), 
                 CV_FILLED, 
                 CV_AA);
    }
    
    [self.delegate frameCaptured:m_frame];
  }
}
            
- (bool) hasMultipleCameras
{
  return false;
}

- (AVCaptureVideoOrientation) videoOrientation
{
  return AVCaptureVideoOrientationLandscapeRight;
}

- (void) toggleCamera
{
}

- (void) startRunning
{
  self.timer = [NSTimer scheduledTimerWithTimeInterval:1 target:self selector:@selector(timerTick:) userInfo:nil repeats:YES];
}

- (void) stopRunning
{
   [self.timer invalidate];
  self.timer = nil;
}

@end

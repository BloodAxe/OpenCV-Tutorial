//
//  VideoSource.m
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "VideoSource.h"

struct Particle
{
  cv::Point2f center;
  cv::Point2f speed;
  cv::Scalar  color;
  int         radius;
 
  Particle()
  : center(-1000,-100)
  , speed(0,0)
  , radius(1)
  {
  }
  cv::Rect boundingRect()
  {
    return cv::Rect(center - cv::Point2f(radius,radius), center + cv::Point2f(radius, radius));
  }
};

@interface DummyVideoSource ()
{
  int     m_frameWidth;
  int     m_frameHeight;
  cv::Mat m_frame;  
  cv::Rect  m_frameRect;
  std::vector<Particle> particles;
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
  
  particles.resize(10);
  m_frameRect = cv::Rect(0, 0, m_frameWidth, m_frameHeight);
  
  return self;
}

-(void) timerTick:(id) sender
{
  if (delegate)
  {
    cv::rectangle(m_frame, cv::Point(0,0), cv::Point(m_frameWidth, m_frameHeight), CV_RGB(0,0,0), CV_FILLED);
    
    for (int i = 0; i < particles.size(); i++)
    {
      Particle& p = particles[i];
      
      cv::Rect r = p.boundingRect() & m_frameRect;
      bool visible = r.area() > 0;
      
      if (!visible)
      {
        p.radius = rand() % (std::min(m_frameHeight, m_frameWidth) / 5);
        p.speed  = cv::Point2f(rand() % 10 + 0.5, 0);
        p.center = cv::Point2f(0, rand() % m_frameHeight);
        p.color  = CV_RGB( rand() % 0xFF, rand() % 0xFF, rand() % 0xFF );
      }
    
      p.center += p.speed;
      cv::circle(m_frame, p.center, p.radius, p.color, CV_FILLED);
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
  self.timer = [NSTimer scheduledTimerWithTimeInterval:0.025 target:self selector:@selector(timerTick:) userInfo:nil repeats:YES];
}

- (void) stopRunning
{
   [self.timer invalidate];
  self.timer = nil;
}

@end

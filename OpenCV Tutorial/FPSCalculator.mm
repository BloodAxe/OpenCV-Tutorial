//
//  FPSCalculator.m
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/4/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "FPSCalculator.h"

#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>

@interface FPSCalculator()
{
  bool         initialized;
  uint64_t     past;
  
  uint64_t     elapsedNano;
  int          elapsedMarks;
  
  float        currentFps;
  
  mach_timebase_info_data_t    sTimebaseInfo;
}
@end

@implementation FPSCalculator

- (id) init
{
  if (self = [super init])
  {
    mach_timebase_info(&sTimebaseInfo);
  }
  
  return self;
}

- (void) putTimeMark
{
  uint64_t now = mach_absolute_time();
  elapsedMarks++;

  if (initialized)
  {
    uint64_t elapsed = now - past;

    elapsedNano += elapsed * sTimebaseInfo.numer / sTimebaseInfo.denom;
  }
  
  initialized = true;
  past = now;
}

- (float) getFPS
{
  if (elapsedNano > 1000000000)
  {
    currentFps = (float)elapsedMarks / (1.0e-9 * elapsedNano);
    elapsedMarks = 0;
    elapsedNano = 0;
  }
  
  return currentFps;
}

- (NSString*) getFPSAsText
{
  int fps = static_cast<int>([self getFPS] + 0.5f);
  
  return [NSString stringWithFormat:@"%d FPS", fps];
}

@end

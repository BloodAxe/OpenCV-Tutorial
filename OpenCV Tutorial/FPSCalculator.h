//
//  FPSCalculator.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/4/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface FPSCalculator : NSObject

- (void) putTimeMark;

- (float) getFPS;

- (NSString*) getFPSAsText;

@end

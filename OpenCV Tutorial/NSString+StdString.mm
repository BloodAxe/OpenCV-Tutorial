//
//  NSString+StdString.m
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/9/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "NSString+StdString.h"

@implementation NSString (StdString)

+ (NSString*) stringWithStdString: (const std::string&) str
{
  return [[NSString alloc] initWithCString:str.c_str() encoding:NSASCIIStringEncoding];
}

@end

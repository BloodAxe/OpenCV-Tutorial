//
//  UIImage_UIImage2OpenCV.mm
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/25/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "UIImage2OpenCV.h"

@implementation UIImage (OpenCV)

-(cv::Mat) toMat
{
  CGImageRef imageRef = self.CGImage;
  
  const int srcWidth        = CGImageGetWidth(imageRef);
  const int srcHeight       = CGImageGetHeight(imageRef);
  const int stride          = CGImageGetBytesPerRow(imageRef);
  const int bitPerPixel     = CGImageGetBitsPerPixel(imageRef);
  const int bitPerComponent = CGImageGetBitsPerComponent(imageRef);
  const int numPixels       = bitPerPixel / bitPerComponent;
  
  CGDataProviderRef dataProvider = CGImageGetDataProvider(imageRef);
  CFDataRef rawData = CGDataProviderCopyData(dataProvider);
  
  unsigned char * dataPtr = const_cast<unsigned char*>(CFDataGetBytePtr(rawData));
  
  cv::Mat view;
  if (numPixels == 4 && bitPerComponent == 8)
  {
    cv::Mat(srcHeight, srcWidth, CV_8UC4, dataPtr, stride).copyTo(view);
  }
  else if(numPixels == 3 && bitPerComponent == 8)
  {
    cv::Mat(srcHeight, srcWidth, CV_8UC3, dataPtr, stride).copyTo(view);
  }
  else if(numPixels == 1 && bitPerComponent == 8) // Assume gray pixel
  {
    cv::Mat(srcHeight, srcWidth, CV_8UC1, dataPtr, stride).copyTo(view);
  }
  else
  {
    NSLog(@"Unsupported format of the input UIImage (neither BGRA, BGR or GRAY)");
  }
    
  CFRelease(rawData);
  return view;
}

+(UIImage*) fromMat:(const cv::Mat&) bitmap
{
  const int bitmapWidth  = bitmap.cols;
  const int bitmapHeight = bitmap.rows;
  
  const int bitsPerComponent = 8;
  const int bitsPerPixel     = 32;
  const int bytesPerRow      = bitmapWidth * bitmapHeight * bitsPerPixel / 8;

  const int dataSize = bitmap.rows * bitmap.cols * bitmap.depth();
  
  CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, bitmap.data, dataSize, NULL);  
  
  CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
  CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault | kCGImageAlphaLast;
  
  CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
  CGImageRef imageRef = CGImageCreate(bitmapWidth,
                           bitmapHeight,
                           bitsPerComponent,
                           bitsPerPixel,
                           bytesPerRow,
                           colorSpaceRef,
                           bitmapInfo,
                           provider,NULL,NO,renderingIntent);
  
  UIImage *newImage = [UIImage imageWithCGImage:imageRef];
  return newImage;

}

@end

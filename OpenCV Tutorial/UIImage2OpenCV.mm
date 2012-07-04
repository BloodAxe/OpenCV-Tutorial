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

+(UIImage*) imageWithMat:(const cv::Mat&) image
{
  
  NSData *data = [NSData dataWithBytes:image.data length:image.elemSize()*image.total()];
  
  CGColorSpaceRef colorSpace;
  
  if (image.elemSize() == 1)
  {
    colorSpace = CGColorSpaceCreateDeviceGray();
  }
  else
  {
    colorSpace = CGColorSpaceCreateDeviceRGB();
  }
  
  bool hasAlpha = image.channels() == 4;
  
  CGDataProviderRef provider = CGDataProviderCreateWithCFData((__bridge CFDataRef)data);
  
  CGBitmapInfo bmInfo = (hasAlpha ? kCGImageAlphaLast : kCGImageAlphaNone) | kCGBitmapByteOrderDefault;
  
  // Creating CGImage from cv::Mat
  CGImageRef imageRef = CGImageCreate(image.cols,                                 //width
                                      image.rows,                                 //height
                                      8,                                          //bits per component
                                      8 * image.elemSize(),                       //bits per pixel
                                      image.step.p[0],                            //bytesPerRow
                                      colorSpace,                                 //colorspace
                                      bmInfo,// bitmap info
                                      provider,                                   //CGDataProviderRef
                                      NULL,                                       //decode
                                      false,                                      //should interpolate
                                      kCGRenderingIntentDefault                   //intent
                                      );
  
  UIImageOrientation imgOrientation = UIImageOrientationUp;
  UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];
 
  switch (orientation) 
  {
    case UIDeviceOrientationLandscapeLeft:
      imgOrientation = UIImageOrientationUp; break;
    
    case UIDeviceOrientationLandscapeRight:
      imgOrientation = UIImageOrientationDown; break;
    
    case UIDeviceOrientationPortraitUpsideDown:
      imgOrientation = UIImageOrientationRightMirrored; break;

    default:
    case UIDeviceOrientationPortrait:
      imgOrientation = UIImageOrientationRight; break;
 };
    
  // Getting UIImage from CGImage
  UIImage *finalImage = [UIImage imageWithCGImage:imageRef scale:1 orientation:imgOrientation];
  CGImageRelease(imageRef);
  CGDataProviderRelease(provider);
  CGColorSpaceRelease(colorSpace);
  
  return finalImage;

}

@end

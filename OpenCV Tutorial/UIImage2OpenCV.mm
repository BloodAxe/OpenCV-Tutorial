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
  //const int stride          = CGImageGetBytesPerRow(imageRef);
  //const int bitPerPixel     = CGImageGetBitsPerPixel(imageRef);
  //const int bitPerComponent = CGImageGetBitsPerComponent(imageRef);
  //const int numPixels       = bitPerPixel / bitPerComponent;
  
  CGDataProviderRef dataProvider = CGImageGetDataProvider(imageRef);
  CFDataRef rawData = CGDataProviderCopyData(dataProvider);
  
  //unsigned char * dataPtr = const_cast<unsigned char*>(CFDataGetBytePtr(rawData));

  CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
  
  cv::Mat rgbaContainer(srcHeight, srcWidth, CV_8UC4);
  CGContextRef context = CGBitmapContextCreate(rgbaContainer.data,
                                               srcWidth,
                                               srcHeight,
                                               8,
                                               4 * srcWidth, 
                                               colorSpace,
                                               kCGImageAlphaNoneSkipLast | kCGBitmapByteOrder32Big);

  CGContextDrawImage(context, CGRectMake(0, 0, srcWidth, srcHeight), imageRef);
  CGContextRelease(context);
  CGColorSpaceRelease(colorSpace);
      
  CFRelease(rawData);
  
  cv::Mat t;
  cv::cvtColor(rgbaContainer, t, CV_RGBA2BGRA);

  //cv::Vec4b a = rgbaContainer.at<cv::Vec4b>(0,0);
  //cv::Vec4b b = t.at<cv::Vec4b>(0,0);
  //std::cout << std::hex << (int)a[0] << " "<< (int)a[1] << " " << (int)a[2] << " "  << (int)a[3] << std::endl; 
  //std::cout << std::hex << (int)b[0] << " "<< (int)b[1] << " " << (int)b[2] << " "  << (int)b[3] << std::endl; 

  return t;
}

+(UIImage*) imageWithMat:(const cv::Mat&) image andDeviceOrientation: (UIDeviceOrientation) orientation
{ 
  UIImageOrientation imgOrientation = UIImageOrientationUp;
  
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
  
  return [UIImage imageWithMat:image andImageOrientation:imgOrientation];
}

+(UIImage*) imageWithMat:(const cv::Mat&) image andImageOrientation: (UIImageOrientation) orientation;
{
  cv::Mat rgbaView;
  
  if (image.channels() == 3)
  {
    cv::cvtColor(image, rgbaView, CV_BGR2RGBA);
  }
  else if (image.channels() == 4)
  {
    cv::cvtColor(image, rgbaView, CV_BGRA2RGBA);
  }
  else if (image.channels() == 1)
  {
    cv::cvtColor(image, rgbaView, CV_GRAY2RGBA);
  }
  
  NSData *data = [NSData dataWithBytes:rgbaView.data length:rgbaView.elemSize() * rgbaView.total()];
  
  CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
  
  CGDataProviderRef provider = CGDataProviderCreateWithCFData((__bridge CFDataRef)data);
  
  CGBitmapInfo bmInfo = kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big;
  
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
    
  // Getting UIImage from CGImage
  UIImage *finalImage = [UIImage imageWithCGImage:imageRef scale:1 orientation:orientation];
  CGImageRelease(imageRef);
  CGDataProviderRelease(provider);
  CGColorSpaceRelease(colorSpace);
  
  return finalImage;
}


- (UIImage *)thumbnailWithSize:(int)thumbSize
{
	// Create a thumbnail version of the image for the event object.
	CGSize croppedSize;
	CGFloat ratio = thumbSize;
	CGFloat offsetX = 0.0;
	CGFloat offsetY = 0.0;
    
	// check the size of the image, we want to make it
	// a square with sides the size of the smallest dimension
	if (self.size.width > self.size.height) {
		offsetX = (self.size.height - self.size.width) / 2;
		croppedSize = CGSizeMake(self.size.height, self.size.height);
	} else {
		offsetY = (self.size.width - self.size.height) / 2;
		croppedSize = CGSizeMake(self.size.width, self.size.width);
	}
    
	// Crop the image before resize
	CGRect clippedRect = CGRectMake(offsetX * -1, offsetY * -1, croppedSize.width, croppedSize.height);
	CGImageRef imageRef = CGImageCreateWithImageInRect([self CGImage], clippedRect);
	// Done cropping
    
	// Resize the image
	CGRect rect = CGRectMake(0.0, 0.0, ratio, ratio);
    
	UIGraphicsBeginImageContext(rect.size);
	[[UIImage imageWithCGImage:imageRef] drawInRect:rect];
	UIImage *thumbnail = UIGraphicsGetImageFromCurrentImageContext();
	UIGraphicsEndImageContext();
	// Done Resizing
    
	return thumbnail;
}
@end

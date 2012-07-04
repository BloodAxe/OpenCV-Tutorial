//
//  ImageViewController.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/4/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SampleBase.h"

@interface ImageViewController : UIViewController <UIImagePickerControllerDelegate, UINavigationControllerDelegate>

- (void) setSample:(SampleBase*) sample;
- (void) setImage:(UIImage*) image;

@property (weak, nonatomic) IBOutlet UIImageView *imageView;

@end

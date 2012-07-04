//
//  DetailViewController.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SampleBase.h"

@interface DetailViewController : UIViewController <UISplitViewControllerDelegate>
{
  SampleBase* currentSample;
  UIImagePickerController * imagePicker;
}

@property (weak, nonatomic) IBOutlet UIImageView *sampleIconView;
@property (weak, nonatomic) IBOutlet UITextView *sampleDescriptionTextView;
@property (weak, nonatomic) IBOutlet UIButton *runOnImageButton;
@property (weak, nonatomic) IBOutlet UIButton *runOnVideoButton;

- (void) setDetailItem:(SampleBase*) sample;
- (void) configureView;

@end

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
}
- (IBAction)startSamplePressed:(id)sender;
@property (weak, nonatomic) IBOutlet UIImageView *sampleIconView;
@property (weak, nonatomic) IBOutlet UITextView *sampleDescriptionTextView;

- (void) setDetailItem:(SampleBase*) sample;

@end

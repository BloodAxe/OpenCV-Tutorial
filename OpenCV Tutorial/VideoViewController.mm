//
//  VideoViewController.m
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "VideoViewController.h"
#import "UIImage2OpenCV.h"

@interface VideoViewController ()
{
  VideoSource * videoSource;
  SampleBase  * currentSample;
  cv::Mat outputFrame;

}

@end

@implementation VideoViewController
@synthesize imageView;
@synthesize toggleCameraButton;

- (void)viewDidLoad
{
  [super viewDidLoad];
  
	// Do any additional setup after loading the view, typically from a nib.
  videoSource = [[VideoSource alloc] init];
  videoSource.delegate = self;
  
  self.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemSave target:self action:@selector(saveProcessingResult:)];
}

- (void) viewWillAppear:(BOOL)animated
{
  [super viewWillAppear:animated];
  [videoSource startRunning];
  toggleCameraButton.hidden = ![videoSource hasMultipleCameras];
}

- (void) viewDidDisappear:(BOOL)animated
{
  [super viewDidDisappear:animated];
  [videoSource stopRunning];
}

- (void) setSample:(SampleBase*) sample
{
  currentSample = sample;
  
  // Configure view
  //title = [currentSample
}

- (IBAction)toggleCameraPressed:(id)sender
{
  [videoSource toggleCamera];
}

- (void) saveProcessingResult:(id) sender
{
  UIImage * image = [UIImage imageWithMat:outputFrame.clone()];
  UIImageWriteToSavedPhotosAlbum(image, self, @selector(image:didFinishSavingWithError:contextInfo:), nil);
}

- (void)image:(UIImage *)image didFinishSavingWithError:(NSError *)error 
  contextInfo:(void *)contextInfo
{
  // Was there an error?
  if (error != NULL)
  {
    // Show error message...
    
  }
  else  // No errors
  {
    // Show message image successfully saved
  }
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
  if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
  } else {
    return YES;
  }
}

#pragma mark -
#pragma mark VideoSourceDelegate

- (void) frameCaptured:(cv::Mat) frame
{
  dispatch_sync( dispatch_get_main_queue(), 
                ^{ 
                  if (currentSample)
                  {
                    currentSample->processFrame(frame, outputFrame);
                    [imageView drawFrame:outputFrame];
                  }
                  
                });
}

- (void)viewDidUnload {
  [self setToggleCameraButton:nil];
  [super viewDidUnload];
}
@end

//
//  VideoViewController.m
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "VideoViewController.h"
#import "UIImage2OpenCV.h"
#import "OptionsTableView.h"

#define kTransitionDuration	0.75

@interface VideoViewController ()
{
#if TARGET_IPHONE_SIMULATOR
  DummyVideoSource * videoSource;
#else
  VideoSource * videoSource;
#endif  
  SampleBase  * currentSample;
  
  cv::Mat outputFrame;
}

@end

@implementation VideoViewController
@synthesize options;
@synthesize imageView;
@synthesize toggleCameraButton;
@synthesize containerView;
@synthesize optionsPopover;
@synthesize optionsView;
@synthesize optionsViewController;

- (void)viewDidLoad
{
  [super viewDidLoad];
    
  // Init the default view (video view layer)
  self.imageView = [[GLESImageView alloc] initWithFrame:self.containerView.bounds];
  [self.imageView setAutoresizingMask:(UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight)];
  [self.containerView addSubview:self.imageView];
  
  // Init video source:
#if TARGET_IPHONE_SIMULATOR
  videoSource = [[DummyVideoSource alloc] initWithFrameSize:CGSizeMake(640, 480)];
#else  
  videoSource = [[VideoSource alloc] init];
#endif
  
  videoSource.delegate = self;
}

- (void) viewWillAppear:(BOOL)animated
{
  [super viewWillAppear:animated];
  
  [videoSource startRunning];
  
  toggleCameraButton.enabled = [videoSource hasMultipleCameras];
}

- (void) viewDidDisappear:(BOOL)animated
{
  [super viewDidDisappear:animated];
  [videoSource stopRunning];
}

- (void) setSample:(SampleBase*) sample
{
  currentSample = sample;
  
  self.optionsView = [[OptionsTableView alloc] initWithFrame:containerView.frame 
                                                       style:UITableViewStyleGrouped 
                                                      sample:sample 
                                       notificationsDelegate:nil];
  
  if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad)
  {
    UIViewController * viewController = [[UIViewController alloc] init];
    viewController.view = self.optionsView;
    viewController.title = @"Algorithm options";
    
    self.optionsViewController = [[UINavigationController alloc] initWithRootViewController:viewController];
    
    self.optionsPopover = [[UIPopoverController alloc] initWithContentViewController:self.optionsViewController];
  }
}

- (IBAction)toggleCameraPressed:(id)sender
{
  [videoSource toggleCamera];
}



- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
  if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone)
  {
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
  }
  else
  {
    return YES;
  }
}



- (void)viewDidUnload
{
  [self setToggleCameraButton:nil];
  [self setContainerView:nil];
  [self setOptions:nil];
  [super viewDidUnload];
}

- (IBAction)showOptions:(id)sender 
{
  if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone)
  {
    if ([self.optionsView superview])
    {
      [UIView transitionFromView:self.optionsView
                          toView:imageView 
                        duration:kTransitionDuration 
                         options:UIViewAnimationOptionTransitionFlipFromLeft 
                      completion:^(BOOL)
       {
       }];
    }
    else
    {
      [self.optionsView setFrame:self.containerView.frame];
      [self.optionsView setNeedsLayout];
      
      [UIView transitionFromView:self.imageView 
                          toView:optionsView 
                        duration:kTransitionDuration 
                         options:UIViewAnimationOptionTransitionFlipFromLeft 
                      completion:^(BOOL)
       {
         
         [self.optionsView reloadData];
         
         NSLog(@"Visible cells count %d" , [[self.optionsView visibleCells] count]);
         NSLog(@"Options view size %fx%f" , self.optionsView.bounds.size.width, self.optionsView.bounds.size.height);
       }];
    }
  }
  else
  {
    if ([self.optionsPopover isPopoverVisible])
      [self.optionsPopover dismissPopoverAnimated:YES];
    else
      [self.optionsPopover presentPopoverFromBarButtonItem:options permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
  }
}

#pragma mark - Image saving

- (IBAction) saveProcessingResult:(id) sender
{
  UIImage * image = [UIImage imageWithMat:outputFrame.clone() andDeviceOrientation:[[UIDevice currentDevice] orientation]];
  UIImageWriteToSavedPhotosAlbum(image, self, @selector(image:didFinishSavingWithError:contextInfo:), nil);
}

- (void)image:(UIImage *)image didFinishSavingWithError:(NSError *)error 
  contextInfo:(void *)contextInfo
{
  if (error != NULL)
  {
    NSLog(@"Error during saving image: %@", error);    
  }
}

#pragma mark - VideoSourceDelegate

- (void) frameCaptured:(cv::Mat) frame
{
  bool isMainQueue = dispatch_get_current_queue() == dispatch_get_main_queue();
  
  if (isMainQueue)
  {
    if (currentSample)
    {
      currentSample->processFrame(frame, outputFrame);
      [imageView drawFrame:outputFrame];
    }
  }
  else
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
  

}

@end

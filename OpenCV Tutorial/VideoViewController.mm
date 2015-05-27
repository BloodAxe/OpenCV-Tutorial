//
//  VideoViewController.m
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/26/12.
//  Copyright (c) 2012 computer-vision-talks.com. All rights reserved.
//

#import "VideoViewController.h"
#import "UIImage2OpenCV.h"
#import "OptionsTableView.h"

#import <opencv2/videoio/cap_ios.h>

#define kTransitionDuration	0.75

@interface VideoViewController ()<CvVideoCameraDelegate>
{
    cv::Mat outputFrame;
}
@property (nonatomic, strong) CvVideoCamera* videoSource;

@end

@implementation VideoViewController

@synthesize videoSource;
@synthesize actionSheetButton;
@synthesize captureReferenceFrameButton;
@synthesize clearReferenceFrameButton;
@synthesize options;
@synthesize toggleCameraButton;
@synthesize containerView;
@synthesize optionsPopover;
@synthesize optionsView;
@synthesize optionsViewController;
@synthesize actionSheet;

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.videoSource = [[CvVideoCamera alloc] initWithParentView:self.containerView];
    self.videoSource.defaultAVCaptureDevicePosition = AVCaptureDevicePositionBack;
    self.videoSource.defaultAVCaptureSessionPreset = AVCaptureSessionPreset1280x720;
    self.videoSource.defaultFPS = 30;
    //self.videoSource.imageWidth = 1280;
    //self.videoSource.imageHeight = 720;
    self.videoSource.delegate = self;
    self.videoSource.recordVideo = NO;
    self.videoSource.grayscaleMode = NO;
    
    self.actionSheet = [[UIActionSheet alloc] initWithTitle:@"Actions"
                                                   delegate:self
                                          cancelButtonTitle:@"Cancel"
                                     destructiveButtonTitle:nil
                                          otherButtonTitles:kSaveImageActionTitle, kComposeTweetWithImage, nil];
    
}

- (void) viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    NSLog(@"capture session loaded: %d", [self.videoSource captureSessionLoaded]);
    
    toggleCameraButton.enabled = true;
    captureReferenceFrameButton.enabled = self.currentSample.isReferenceFrameRequired;
    clearReferenceFrameButton.enabled   = self.currentSample.isReferenceFrameRequired;
}

- (void) viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    
    [self.videoSource start];
    
    //[self.videoSource adjustLayoutToInterfaceOrientation:self.interfaceOrientation];
    
}

- (void) viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
    [self.videoSource stop];
}

- (void) configureView
{
    [super configureView];
    
    self.optionsView = [[OptionsTableView alloc] initWithFrame:containerView.frame
                                                         style:UITableViewStyleGrouped
                                                        sample:self.currentSample
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
    [self.videoSource switchCameras];
}

- (IBAction)showActionSheet:(id)sender
{
    if ([self.actionSheet isVisible])
        [self.actionSheet dismissWithClickedButtonIndex:-1 animated:YES];
    else
        [self.actionSheet showFromBarButtonItem:self.actionSheetButton animated:YES];
}

- (void)viewDidUnload
{
    [self setToggleCameraButton:nil];
    [self setContainerView:nil];
    [self setOptions:nil];
    [self setActionSheetButton:nil];
    [self setCaptureReferenceFrameButton:nil];
    [self setClearReferenceFrameButton:nil];
    
    [super viewDidUnload];
}

- (IBAction)showOptions:(id)sender
{
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone)
    {
        if ([self.optionsView superview])
        {
            [UIView transitionFromView:self.optionsView
                                toView:self.containerView
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
            
            [UIView transitionFromView:self.containerView
                                toView:self.optionsView
                              duration:kTransitionDuration
                               options:UIViewAnimationOptionTransitionFlipFromLeft
                            completion:^(BOOL)
             {
                 [self.optionsView reloadData];
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

#pragma mark - Protocol CvVideoCameraDelegate

#ifdef __cplusplus
- (void) processImage:(cv::Mat&)image
{
    // Do some OpenCV stuff with the image
    [self.currentSample processFrame:image into:outputFrame];
    
    //outputFrame.copyTo(image);
}
#endif


#pragma mark UIActionSheetDelegate implementation

- (void)actionSheet:(UIActionSheet *)senderSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
    NSString * title = [senderSheet buttonTitleAtIndex:buttonIndex];
    
    if ([title  isEqual: kSaveImageActionTitle])
    {
        UIImage * image = [UIImage imageWithMat:outputFrame.clone() andDeviceOrientation:[[UIDevice currentDevice] orientation]];
        [self saveImage:image withCompletionHandler: ^{ [self.videoSource start]; }];
    }
    else if ([title  isEqual: kComposeTweetWithImage])
    {
        UIImage * image = [UIImage imageWithMat:outputFrame.clone() andDeviceOrientation:[[UIDevice currentDevice] orientation]];
        [self tweetImage:image withCompletionHandler:^{ [self.videoSource start]; }];
    }
    else
    {
        [self.videoSource start];
    }
}

- (void)willPresentActionSheet:(UIActionSheet *)actionSheet;  // before animation and showing view
{
    [self.videoSource stop];
}

- (void) didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
    [super didRotateFromInterfaceOrientation: fromInterfaceOrientation];
    [self.videoSource adjustLayoutToInterfaceOrientation:self.interfaceOrientation];
}

#pragma mark - Capture reference frame

- (IBAction) captureReferenceFrame:(id) sender
{
    dispatch_async( dispatch_get_main_queue(),
                   ^{
                       [self.currentSample setReferenceFrame:outputFrame];
                   });
}

#pragma mark - Clear reference frame

- (IBAction) clearReferenceFrame:(id) sender
{
    dispatch_async(dispatch_get_main_queue(),
                   ^{
                       [self.currentSample resetReferenceFrame];
                   });
}

@end

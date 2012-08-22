//
//  VideoViewController.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "GLESImageView.h"
#import "VideoSource.h"
#import "SampleBase.h"
#import "BaseSampleViewController.h"

@interface VideoViewController : BaseSampleViewController<VideoSourceDelegate,UIActionSheetDelegate>

@property (weak, nonatomic) IBOutlet UIView *containerView;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *toggleCameraButton;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *options;

@property (nonatomic, strong) GLESImageView *imageView;
@property (nonatomic, strong) UITableView * optionsView;
@property (nonatomic, strong) UIPopoverController * optionsPopover;
@property (nonatomic, strong) UIViewController * optionsViewController;

@property (nonatomic, strong) UIActionSheet * actionSheet;

- (IBAction)toggleCameraPressed:(id)sender;
- (IBAction)showOptions:(id)sender;
- (IBAction)captureReferenceFrame:(id)sender;
- (IBAction)clearReferenceFrame:(id)sender;

@property (weak, nonatomic) IBOutlet UIBarButtonItem *actionSheetButton;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *captureReferenceFrameButton;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *clearReferenceFrameButton;

- (IBAction)showActionSheet:(id)sender;

@end


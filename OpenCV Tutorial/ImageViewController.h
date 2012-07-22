//
//  ImageViewController.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/4/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SampleBase.h"
#import "OptionsTableView.h"
#import "OptionCell.h"
#import "BaseSampleViewController.h"


@interface ImageViewController : BaseSampleViewController <UIImagePickerControllerDelegate, UINavigationControllerDelegate, OptionCellDelegate, UIActionSheetDelegate>

- (void) setImage:(UIImage*) image;

@property (nonatomic, strong) UIImageView *imageView;
@property (nonatomic, strong) OptionsTableView *optionsView;
@property (nonatomic, strong) UIPopoverController * optionsPopover;
@property (nonatomic, strong) UIViewController * optionsViewController;
@property (nonatomic, strong) UIActionSheet * actionSheet;

@property (weak, nonatomic) IBOutlet UIView *containerView;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *takePhotoButton;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *optionsBarButton;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *actionButton;

- (IBAction) presentOptionsView:(id)sender;
- (IBAction) selectPictureForProcessing:(id) sender;
- (IBAction) selectAction:(id)sender;


@end

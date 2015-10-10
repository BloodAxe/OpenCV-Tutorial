//
//  ImageViewController.m
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/4/12.
//  Copyright (c) 2012 computer-vision-talks.com. All rights reserved.
//

#import "ImageViewController.h"
#import "UIImage2OpenCV.h"
#import "SampleOptionsTableViewDelegate.h"
#import "NSString+StdString.h"

#import <AssetsLibrary/ALAssetsLibrary.h>
#import <AssetsLibrary/ALAssetRepresentation.h>

#define kTransitionDuration	0.75

#define kSaveImageActionTitle  @"Save image"

@interface ImageViewController ()
{
    UIImage     * currentImage;
}

- (void) configureView;
- (void) updateImageView;

@property (strong, nonatomic) UIImagePickerController *imagePickerController;
@property (strong, nonatomic) UIPopoverController *imagePickerPopoverController;

@end

@implementation ImageViewController
@synthesize actionButton;
@synthesize containerView;
@synthesize optionsBarButton;
@synthesize imageView;
@synthesize imagePickerPopoverController;
@synthesize imagePickerController;
@synthesize optionsView;
@synthesize takePhotoButton;
@synthesize optionsPopover;
@synthesize optionsViewController;

@synthesize actionSheet;

#pragma mark - View Lifecycle

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.imagePickerController = [[UIImagePickerController alloc] init];
    self.imagePickerController.delegate = self;
    self.imagePickerController.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad)
    {
        self.imagePickerPopoverController = [[UIPopoverController alloc] initWithContentViewController:self.imagePickerController];
    }
    
    // create the initial image view
    self.imageView = [[UIImageView alloc] initWithFrame:self.containerView.bounds];
    [self.imageView setAutoresizingMask:(UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight)];
    
    [self.containerView addSubview:self.imageView];
    
  self.actionSheet = [UIAlertController alertControllerWithTitle:@"Actions"
                                                         message:nil
                                                  preferredStyle:UIAlertControllerStyleActionSheet];
  UIAlertAction *firstAction = [UIAlertAction actionWithTitle:kSaveImageActionTitle
                                                        style:UIAlertActionStyleDefault handler:^(UIAlertAction * action) {
                                                          [self saveImage:self.imageView.image withCompletionHandler:nil];
                                                        }];

  [self.actionSheet addAction:firstAction];
}

- (void)viewDidUnload
{
    [self setImageView:nil];
    [self setContainerView:nil];
    [self setTakePhotoButton:nil];
    [self setOptionsBarButton:nil];
    [self setActionButton:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

- (void) viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    NSLog(@"Container view dimensions before appear: %f x %f", self.containerView.frame.size.width, self.containerView.frame.size.height);
    
}



#pragma mark - Sample management

- (void) setImage:(UIImage*) image
{
    currentImage = image;
    [self updateImageView];
}

- (void) configureView
{
    [super configureView];
    
    self.optionsView = [[OptionsTableView alloc] initWithFrame:containerView.frame
                                                         style:UITableViewStyleGrouped
                                                        sample:self.currentSample
                                         notificationsDelegate:self];
    
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad)
    {
        UIViewController * viewController = [[UIViewController alloc] init];
        viewController.view = self.optionsView;
        viewController.title = @"Algorithm options";
        
        self.optionsViewController = [[UINavigationController alloc] initWithRootViewController:viewController];
        
        // Limit options popover dimensions. Todo make a popover automaticaly resize depending on number of registered properties
        [viewController setPreferredContentSize:CGSizeMake(320, 500)];
        
        self.optionsPopover = [[UIPopoverController alloc] initWithContentViewController:self.optionsViewController];
    }
    
    [self updateImageView];
}

- (void) updateImageView
{
    if (self.currentSample && currentImage)
    {
        self.imageView.image = [self.currentSample processFrame:currentImage];
    }
}

#pragma mark - Handling user interaction

- (IBAction) selectPictureForProcessing:(id)sender
{
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad)
    {
        [self.optionsPopover dismissPopoverAnimated:YES];
        [self.imagePickerPopoverController presentPopoverFromBarButtonItem:takePhotoButton
                                                  permittedArrowDirections:UIPopoverArrowDirectionAny
                                                                  animated:YES];
    }
    else
    {
        [self presentViewController:imagePickerController animated:YES completion:nil];
    }
}

- (IBAction)presentOptionsView:(id)sender
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
                 
                 NSLog(@"Visible cells count %lu" , (unsigned long)[[self.optionsView visibleCells] count]);
                 NSLog(@"Options view size %fx%f" , self.optionsView.bounds.size.width, self.optionsView.bounds.size.height);
             }];
        }
    }
    else
    {
        if ([self.optionsPopover isPopoverVisible])
            [self.optionsPopover dismissPopoverAnimated:YES];
        else
        {
            [self.imagePickerPopoverController dismissPopoverAnimated:YES];
            [self.optionsPopover presentPopoverFromBarButtonItem:optionsBarButton
                                        permittedArrowDirections:UIPopoverArrowDirectionAny
                                                        animated:YES];
        }
    }
}

- (IBAction)selectAction:(id)sender
{
  [self presentViewController:self.actionSheet animated:YES completion:nil];
}

#pragma mark - UIImagePickerControllerDelegate implementation

- (void) imagePickerController:(UIImagePickerController *)picker
 didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone)
    {
        [picker dismissViewControllerAnimated:YES completion:nil];
    }
    else
    {
        [self.imagePickerPopoverController dismissPopoverAnimated:YES];
    }
    
    UIImage * editImage = [info objectForKey:UIImagePickerControllerEditedImage];
    if (editImage != nil)
    {
        [self setImage:editImage];
        return;
    }
    
    UIImage * origImage = [info objectForKey:UIImagePickerControllerOriginalImage];
    if (origImage != nil)
    {
        [self setImage:origImage];
        return;
    }
    
    NSURL * imageUrl = [info objectForKey:UIImagePickerControllerReferenceURL];
    if (imageUrl != nil)
    {
        ALAssetsLibrary *assetLibrary = [[ALAssetsLibrary alloc]init];
        ALAssetsLibraryAssetForURLResultBlock resultblock = ^(ALAsset *myasset)
        {
            ALAssetRepresentation *rep = [myasset defaultRepresentation];
            CGImageRef iref = [rep fullResolutionImage];
            
            if (iref != nil)
            {
                NSLog(@"Loaded image from assets library");
                UIImage * pickedImage = [[UIImage alloc] initWithCGImage:iref];
                [self setImage:pickedImage];
            }
        };
        
        ALAssetsLibraryAccessFailureBlock failureblock = ^(NSError *myerror)
        {
            NSLog(@"failed");
        };
        
        [assetLibrary assetForURL:imageUrl
                      resultBlock:resultblock
                     failureBlock:failureblock];
        
    }
    
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone)
    {
        [picker dismissViewControllerAnimated:YES completion:nil];
    }
    else
    {
        [self.imagePickerPopoverController dismissPopoverAnimated:YES];
    }
}

#pragma mark - OptionCellDelegate implementation

- (void) optionDidChanged:(SampleOption*) option
{
    [self updateImageView];
}

@end

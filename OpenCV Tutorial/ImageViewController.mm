//
//  ImageViewController.m
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/4/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "ImageViewController.h"
#import "UIImage2OpenCV.h"
#import "SampleOptionsTableViewDelegate.h"

#import <AssetsLibrary/ALAssetsLibrary.h>
#import <AssetsLibrary/ALAssetRepresentation.h>

#define kTransitionDuration	0.75

@interface ImageViewController ()
{
  SampleBase  * currentSample;
  UIImage     * currentImage;
}

- (void) configureView;
- (void) updateImageView;

@property (strong, nonatomic) UIImagePickerController *imagePickerController;
@property (strong, nonatomic) UIPopoverController *imagePickerPopoverController;

@end

@implementation ImageViewController
@synthesize containerView;
@synthesize optionsBarButton;
@synthesize imageView;
@synthesize imagePickerPopoverController;
@synthesize imagePickerController;
@synthesize optionsView;
@synthesize takePhotoButton;
@synthesize optionsPopover;
@synthesize optionsViewController;

#pragma mark - View Lifecycle
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
  self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
  if (self)
  {
    
  }
  return self;
}

- (void)viewDidLoad
{
  [super viewDidLoad];
  
  self.imagePickerController = [[UIImagePickerController alloc] init];
  self.imagePickerController.delegate = self;
  self.imagePickerController.sourceType = UIImagePickerControllerSourceTypeSavedPhotosAlbum;
  
  if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad)
  {
    self.imagePickerPopoverController = [[UIPopoverController alloc] initWithContentViewController:self.imagePickerController];
  }
  
  
  // create the initial image view
	self.imageView = [[UIImageView alloc] initWithFrame:self.containerView.bounds];
  [self.imageView setAutoresizingMask:(UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight)];
  
	[self.containerView addSubview:self.imageView];
}

- (void)viewDidUnload
{
  [self setImageView:nil];
  [self setContainerView:nil];
  [self setTakePhotoButton:nil];
  [self setOptionsBarButton:nil];
  [super viewDidUnload];
  // Release any retained subviews of the main view.
}

- (void) viewWillAppear:(BOOL)animated
{
  [super viewWillAppear:animated];
  NSLog(@"Container view dimensions before appear: %f x %f", self.containerView.frame.size.width, self.containerView.frame.size.height);
  
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

#pragma mark - Sample management

- (void) setSample:(SampleBase*) sample
{
  currentSample = sample;
  
  self.optionsView = [[OptionsTableView alloc] initWithFrame:containerView.frame style:UITableViewStyleGrouped sample:sample notificationsDelegate:self];
  //self.optionsView.
  
  if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad)
  {
    UIViewController * viewController = [[UIViewController alloc] init];
    viewController.view = self.optionsView;
    viewController.title = @"Algorithm options";
    
    self.optionsViewController = [[UINavigationController alloc] initWithRootViewController:viewController];
    
    self.optionsPopover = [[UIPopoverController alloc] initWithContentViewController:self.optionsViewController];
  }
  
  [self configureView];
}

- (void) setImage:(UIImage*) image
{
  currentImage = image;
  [self updateImageView];
}

- (void) configureView
{
  if (currentSample)
  {
    self.title = [NSString stringWithCString:currentSample->getName().c_str() encoding:NSASCIIStringEncoding];
    [self updateImageView];
  }
}

- (void) updateImageView
{
  if (currentSample && currentImage)
  {
    cv::Mat inputImage = [currentImage toMat];
    cv::Mat outputImage;
    
    currentSample->processFrame(inputImage, outputImage);
    UIImage * result = [UIImage imageWithMat:outputImage andImageOrientation:[currentImage imageOrientation]];
    
    self.imageView.image = result;
  }
}

#pragma mark - Handling user interaction

- (IBAction) tookPicture:(id) sender
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
    [self presentModalViewController:imagePickerController animated:YES];    
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
    {
      [self.imagePickerPopoverController dismissPopoverAnimated:YES];
      [self.optionsPopover presentPopoverFromBarButtonItem:optionsBarButton 
                                  permittedArrowDirections:UIPopoverArrowDirectionAny 
                                                  animated:YES];
    }
  }
}

#pragma mark - UIImagePickerControllerDelegate implementation

- (void) imagePickerController:(UIImagePickerController *)picker 
 didFinishPickingMediaWithInfo:(NSDictionary *)info
{  
  if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) 
  {
    [picker dismissModalViewControllerAnimated:YES];
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
    [picker dismissModalViewControllerAnimated:YES];
  }
  else
  {
    [self.imagePickerPopoverController dismissPopoverAnimated:YES];
  }
}

#pragma mark - Image saving

- (IBAction) saveProcessingResult:(id) sender
{
  UIImage * image = self.imageView.image;
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

#pragma mark - OptionCellDelegate implementation

- (void) optionDidChanged:(SampleOption*) option
{
  [self updateImageView];
}

@end

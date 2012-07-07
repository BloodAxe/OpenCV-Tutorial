//
//  ImageViewController.m
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/4/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "ImageViewController.h"
#import "UIImage2OpenCV.h"

#import <AssetsLibrary/ALAssetsLibrary.h>
#import <AssetsLibrary/ALAssetRepresentation.h>

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
@synthesize imageView;
@synthesize imagePickerPopoverController;
@synthesize imagePickerController;

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
	// Do any additional setup after loading the view.
  self.navigationItem.rightBarButtonItems = [NSArray arrayWithObjects:
                                             [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemSave target:self action:@selector(saveProcessingResult:)],
                                             [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemCamera target:self action:@selector(tookPicture:)],
                                             nil];
  
  self.imagePickerController = [[UIImagePickerController alloc] init];
  self.imagePickerController.delegate = self;
  self.imagePickerController.sourceType = UIImagePickerControllerSourceTypeSavedPhotosAlbum;
  
  if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad)
  {
    self.imagePickerPopoverController = [[UIPopoverController alloc] initWithContentViewController:self.imagePickerController];
  }
}

- (void)viewDidUnload
{
  [self setImageView:nil];
  [super viewDidUnload];
  // Release any retained subviews of the main view.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
  if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
  } else {
    return YES;
  }
}

#pragma mark - Sample management

- (void) setSample:(SampleBase*) sample
{
  currentSample = sample;
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

- (void) tookPicture:(id) sender
{
  if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad)
  {
    UIBarButtonItem * btn = [self.navigationItem.rightBarButtonItems objectAtIndex:0];
    [self.imagePickerPopoverController presentPopoverFromBarButtonItem:btn permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
  }
  else
  {
    [self presentModalViewController:imagePickerController animated:YES];    
  }
}

#pragma mark - Image saving

- (void) saveProcessingResult:(id) sender
{
  UIImage * image = self.imageView.image;
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
@end

//
//  BaseSampleViewController.m
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/20/12.
//  Copyright (c) 2012 computer-vision-talks.com. All rights reserved.
//

#import "BaseSampleViewController.h"
#import "NSString+StdString.h"
#import "UIImage2OpenCV.h"

@interface BaseSampleViewController ()

@end

@implementation BaseSampleViewController
@synthesize currentSample = _currentSample;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self)
    {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

- (void) setSample:(SampleFacade*) sample
{
  _currentSample = sample;
  
  [self configureView];
}

- (void) configureView
{
  if (self.currentSample)
  {
      self.title = [self.currentSample title];
  }
}

#pragma mark - Image Saving

- (void) saveImage:(UIImage *) image   withCompletionHandler: (SaveImageCompletionHandler) handler
{
  UIImageWriteToSavedPhotosAlbum(image, self, @selector(image:didFinishSavingWithError:contextInfo:), nil);
  if (handler)
    handler();
}

- (void)image:(UIImage *)image didFinishSavingWithError:(NSError *)error contextInfo:(void *)contextInfo
{
  if (error != NULL)
  {
    NSLog(@"Error during saving image: %@", error);    
  }
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


@end

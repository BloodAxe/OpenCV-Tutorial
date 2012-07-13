//
//  DetailViewController.m
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "DetailViewController.h"
#import "VideoViewController.h"
#import "ImageViewController.h"
#import "NSString+StdString.h"

@interface DetailViewController ()
@property (strong, nonatomic) UIPopoverController *masterPopoverController;

- (void)configureView;
@end

@implementation DetailViewController
@synthesize sampleIconView;
@synthesize sampleDescriptionTextView;
@synthesize runOnImageButton;
@synthesize runOnVideoButton;
@synthesize masterPopoverController = _masterPopoverController;

#pragma mark - Managing the detail item

- (void)setDetailItem:(SampleBase*) sample
{
  if (currentSample != sample)
  {
    currentSample = sample;
    [self configureView];
  }
  
  if (self.masterPopoverController != nil)
  {
    [self.masterPopoverController dismissPopoverAnimated:YES];
  }        
}

- (void)configureView
{
  // Update the user interface for the detail item.
  
  if (currentSample)
  {    
    self.sampleDescriptionTextView.text = [NSString stringWithStdString:currentSample->getDescription()];
    self.title = [NSString stringWithStdString:currentSample->getName()];     
    
    if (currentSample->hasIcon())
    {
      NSString * iconStr = [NSString stringWithStdString:currentSample->getSampleIcon()];
      self.sampleIconView.image = [UIImage imageNamed:iconStr];
    }
    else
    {
      self.sampleIconView.image = nil;
    }
  }
}

- (void)viewDidLoad
{
  [super viewDidLoad];
  
	// Do any additional setup after loading the view, typically from a nib.
  [self configureView];
}

- (void)viewDidUnload
{
  [self setSampleIconView:nil];
  [self setSampleDescriptionTextView:nil];
  [self setRunOnImageButton:nil];
  [self setRunOnVideoButton:nil];
  [super viewDidUnload];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
  if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
  } else {
    return YES;
  }
}

#pragma mark - Split view

- (void)splitViewController:(UISplitViewController *)splitController willHideViewController:(UIViewController *)viewController withBarButtonItem:(UIBarButtonItem *)barButtonItem forPopoverController:(UIPopoverController *)popoverController
{
  barButtonItem.title = NSLocalizedString(@"Master", @"Master");
  [self.navigationItem setLeftBarButtonItem:barButtonItem animated:YES];
  self.masterPopoverController = popoverController;
}

- (void)splitViewController:(UISplitViewController *)splitController willShowViewController:(UIViewController *)viewController invalidatingBarButtonItem:(UIBarButtonItem *)barButtonItem
{
  // Called when the view is shown again in the split view, invalidating the button and popover controller.
  [self.navigationItem setLeftBarButtonItem:nil animated:YES];
  self.masterPopoverController = nil;
}

#pragma mark - Run Sample

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
  if ([[segue identifier] isEqualToString:@"processVideo"])
  {
    VideoViewController * sampleController = [segue destinationViewController];
    [sampleController setSample:currentSample];
    
    
  }
  else if ([[segue identifier] isEqualToString:@"processImage"])
  {
    ImageViewController * sampleController = [segue destinationViewController];
    [sampleController setSample:currentSample];    
  }
}




@end

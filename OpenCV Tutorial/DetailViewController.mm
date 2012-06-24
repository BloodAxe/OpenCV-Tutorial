//
//  DetailViewController.m
//  OpenCV Tutorial
//
//  Created by BloodAxe on 6/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "DetailViewController.h"

@interface DetailViewController ()
@property (strong, nonatomic) UIPopoverController *masterPopoverController;
- (void)configureView;
@end

@implementation DetailViewController
@synthesize sampleIconView;
@synthesize sampleDescriptionTextView;

@synthesize masterPopoverController = _masterPopoverController;

#pragma mark - Managing the detail item

- (IBAction)startSamplePressed:(id)sender {
}

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
    std::string name = currentSample->getName();
    std::string desc = currentSample->getDescription();
    std::string icon = currentSample->getSampleIcon();

    NSString * nameStr = [NSString stringWithCString:desc.c_str() encoding:NSASCIIStringEncoding];
    NSString * descStr = [NSString stringWithCString:name.c_str() encoding:NSASCIIStringEncoding];

    self.sampleDescriptionTextView.text = descStr;
    self.title = nameStr;     
    
    if (!icon.empty())
    {
      NSString * iconStr = [NSString stringWithCString:icon.c_str() encoding:NSASCIIStringEncoding];
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

@end

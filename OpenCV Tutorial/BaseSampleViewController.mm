//
//  BaseSampleViewController.m
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/20/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "BaseSampleViewController.h"
#import "NSString+StdString.h"
#import "UIImage2OpenCV.h"

#import <Twitter/Twitter.h>

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

#pragma mark - Image Tweeting

- (void) tweetImage:(UIImage*) image withCompletionHandler: (TweetImageCompletionHandler) handler
{
  // Create the view controller
  TWTweetComposeViewController *twitter = [[TWTweetComposeViewController alloc] init];
  
  NSString * text = [NSString stringWithFormat:@"%@ with OpenCV Tutorial made by @cvtalks", 
                     [self.currentSample friendlyName]];
  
  [twitter addImage:image];
  [twitter addURL:[NSURL URLWithString:@"http://computer-vision-talks.com/"]];
  [twitter setInitialText:text];
  
  // Show the controller
  [self presentModalViewController:twitter animated:YES];
  
  // Called when the tweet dialog has been closed
  twitter.completionHandler = ^(TWTweetComposeViewControllerResult result) 
  {
    NSString *title = @"Tweet Status";
    NSString *msg; 
    
    if (result == TWTweetComposeViewControllerResultDone)
    {
      msg = @"Picture was tweeted.";
      
      // Show alert to see how things went...
      UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:title message:msg delegate:self cancelButtonTitle:@"Okay" otherButtonTitles:nil];
      [alertView show];
    }
    
    // Dismiss the controller
    [self dismissModalViewControllerAnimated:YES];
    
    if (handler != nil)
      handler();
  };

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

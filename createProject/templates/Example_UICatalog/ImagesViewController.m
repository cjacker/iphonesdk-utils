/*

File: ImagesViewController.m
Abstract: The view controller for hosting the UIImageView containing multiple
images.

Version: 1.7

Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc.
("Apple") in consideration of your agreement to the following terms, and your
use, installation, modification or redistribution of this Apple software
constitutes acceptance of these terms.  If you do not agree with these terms,
please do not use, install, modify or redistribute this Apple software.

In consideration of your agreement to abide by the following terms, and subject
to these terms, Apple grants you a personal, non-exclusive license, under
Apple's copyrights in this original Apple software (the "Apple Software"), to
use, reproduce, modify and redistribute the Apple Software, with or without
modifications, in source and/or binary forms; provided that if you redistribute
the Apple Software in its entirety and without modifications, you must retain
this notice and the following text and disclaimers in all such redistributions
of the Apple Software.
Neither the name, trademarks, service marks or logos of Apple Inc. may be used
to endorse or promote products derived from the Apple Software without specific
prior written permission from Apple.  Except as expressly stated in this notice,
no other rights or licenses, express or implied, are granted by Apple herein,
including but not limited to any patent rights that may be infringed by your
derivative works or by other works in which the Apple Software may be
incorporated.

The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
COMBINATION WITH YOUR PRODUCTS.

IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR
DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF
CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Copyright (C) 2008 Apple Inc. All Rights Reserved.

*/

#import "ImagesViewController.h"
#import "Constants.h"

@implementation ImagesViewController

#define kMinDuration 0.0
#define kMaxDuration 10.0

- (id)init
{
	self = [super init];
	if (self)
	{
		// this title will appear in the navigation bar
		self.title = NSLocalizedString(@"ImagesTitle", @"");
	}
	return self;
}

- (void)dealloc
{
	[imageView release];
	[super dealloc];
}

- (void)loadView
{	
	// setup our parent content view and embed it to your view controller
	UIView *contentView = [[UIView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]];
	contentView.backgroundColor = [UIColor blackColor];
	contentView.autoresizesSubviews = YES;
	self.view = contentView;
	[contentView release];
	
	// set up our UIImage with a group or array of images to animate (or in our case a slideshow)
	CGRect frame = CGRectMake(	kLeftMargin,
								kTopMargin,
								self.view.bounds.size.width - (kRightMargin * 2.0),
								self.view.bounds.size.height - 240.0);
	imageView = [[UIImageView alloc] initWithFrame:frame];
	imageView.animationImages = [NSArray arrayWithObjects:	[UIImage imageNamed:@"scene1.jpg"],
															[UIImage imageNamed:@"scene2.jpg"],
															[UIImage imageNamed:@"scene3.jpg"],
															[UIImage imageNamed:@"scene4.jpg"],
															[UIImage imageNamed:@"scene5.jpg"], nil];
	imageView.animationDuration = 5.0;
	[imageView stopAnimating];
	[self.view addSubview:imageView];

	// duration slider
	frame = CGRectMake(	kLeftMargin,
						self.view.bounds.size.height - 90.0,
						self.view.bounds.size.width - (kRightMargin * 2.0),
						kSliderHeight);
	UISlider *slider = [[UISlider alloc] initWithFrame:frame];
	[slider addTarget:self action:@selector(sliderAction:) forControlEvents:UIControlEventValueChanged];
	// in case the parent view draws with a custom color or gradient, use a transparent color
	slider.backgroundColor = [UIColor clearColor];
	slider.minimumValue = kMinDuration;
	slider.maximumValue = kMaxDuration;
	slider.continuous = YES;
	slider.value = kMaxDuration / 2.0;
	[self.view addSubview:slider];
    [slider release];
	
	// duration label
	frame = CGRectMake(	kLeftMargin,
						self.view.bounds.size.height - 67.0,
						self.view.bounds.size.width - (kRightMargin * 2.0),
						kLabelHeight);
	UILabel *label = [[UILabel alloc] initWithFrame:frame];
    label.font = [UIFont systemFontOfSize:12.0];

    IF_PRE_IOS6 (
      label.textAlignment = UITextAlignmentCenter;
    )
    IF_IOS6_OR_GREATER (
      label.textAlignment = NSTextAlignmentCenter;
    )
	label.text = @"Duration";
	label.textColor = [UIColor whiteColor];
	label.backgroundColor = [UIColor clearColor];
	[self.view addSubview:label];
	[label release];
}

// slown down or speed up the slide show as the slider is moved
- (void)sliderAction:(id)sender
{
	UISlider* slider = sender;
	imageView.animationDuration = [slider value];
	if (!imageView.isAnimating)
		[imageView startAnimating];
}


#pragma mark UIViewController delegate methods

// called after this controller's view was dismissed, covered or otherwise hidden
- (void)viewWillDisappear:(BOOL)animated
{	
	[imageView stopAnimating];
	
	// restore the nav bar and status bar color to default
	self.navigationController.navigationBar.barStyle = UIBarStyleDefault;
	[UIApplication sharedApplication].statusBarStyle = UIStatusBarStyleDefault;
}

// called after this controller's view will appear
- (void)viewWillAppear:(BOOL)animated
{	
	[imageView startAnimating];
	
	// for aesthetic reasons (the background is black), make the nav bar black for this particular page
	self.navigationController.navigationBar.barStyle = UIBarStyleBlackOpaque;
	
	// match the status bar with the nav bar
	[UIApplication sharedApplication].statusBarStyle = UIStatusBarStyleBlackOpaque;
}

@end


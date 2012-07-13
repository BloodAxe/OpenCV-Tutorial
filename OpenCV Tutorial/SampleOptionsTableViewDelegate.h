//
//  SampleOptionsTableViewDelegate.h
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/9/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "SampleBase.h"
#import "OptionCell.h"

@interface SampleOptionsTableViewDelegate : NSObject<UITableViewDelegate, UITableViewDataSource>

@property (readonly) SampleBase * sample;
@property (readonly) id<OptionCellDelegate> delegate;

- (id) initWithSample:(SampleBase*) sample notificationsDelegate:(id<OptionCellDelegate>) delegate;

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath;
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView;
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section;
- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section;

- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath;
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath;

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath;
- (NSIndexPath *)tableView:(UITableView *)tableView willSelectRowAtIndexPath:(NSIndexPath *)indexPath;

@end

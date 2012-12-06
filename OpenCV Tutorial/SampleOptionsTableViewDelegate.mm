//
//  SampleOptionsTableViewDelegate.m
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/9/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "SampleOptionsTableViewDelegate.h"
#import "NSString+StdString.h"
#import "BooleanTableViewCell.h"
#import "Int32TableViewCell.h"
#import "FloatTableViewCell.h"
#import "EnumTableViewCell.h"
#import "DoubleTableViewCell.h"
#import "SampleBase.h"

@interface SampleOptionsTableViewDelegate ()
{
  std::vector<std::string> sectionLabels;
  std::vector< SampleBase::OptionsSection > sections;
}
@end

@implementation SampleOptionsTableViewDelegate
@synthesize sample = _sample;
@synthesize delegate = _delegate;

- (id) initWithSample:(SampleFacade*) targetSample notificationsDelegate:(id<OptionCellDelegate>) targetDelegate
{
  if (self == [super init])
  {
    _sample = targetSample;
    _delegate = targetDelegate;
    
    const OptionsMap& options = [_sample getOptions];
    
    for (OptionsMap::const_iterator i = options.begin(); i != options.end(); ++i)
    {
      sections.push_back(i->second);
      sectionLabels.push_back(i->first);
    }
  }
  
  return self;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
  SampleOption * option = sections[indexPath.section][indexPath.row];

  switch (option->getKind())
  {
    case OptionKindBoolean:
    {
      BooleanTableViewCell * cell = [tableView dequeueReusableCellWithIdentifier:@"BooleanTableViewCell"];
      [cell configureWithOption:dynamic_cast<BooleanOption*>(option)];
      cell.delegate = self.delegate;
      return cell;
    }
      
    case OptionKindInt32:
    {
      Int32TableViewCell * cell = [tableView dequeueReusableCellWithIdentifier:@"Int32TableViewCell"];
      [cell configureWithOption:dynamic_cast<Int32Option*>(option)];      
      cell.delegate = self.delegate;
      return cell;
    }
      
    case OptionKindFloat:
    {
      FloatTableViewCell * cell = [tableView dequeueReusableCellWithIdentifier:@"FloatTableViewCell"];
      [cell configureWithOption:dynamic_cast<FloatOption*>(option)];      
      cell.delegate = self.delegate;
      return cell;
    }
    case OptionKindDouble:
    {
      DoubleTableViewCell * cell = [tableView dequeueReusableCellWithIdentifier:@"DoubleTableViewCell"];
      [cell configureWithOption:dynamic_cast<DoubleOption*>(option)];      
      cell.delegate = self.delegate;
      return cell;
    }
      
    case OptionKindStringEnum:
    {
      EnumTableViewCell * cell = [tableView dequeueReusableCellWithIdentifier:@"EnumTableViewCell"];
      [cell configureWithOption:dynamic_cast<StringEnumOption*>(option)];      
      cell.delegate = self.delegate;
      return cell;
    }
      
    default:
      return nil;
  }
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
  return sections.size();  
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
  return sections[section].size();    
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
  return [NSString stringWithStdString: sectionLabels[section]];
}

- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath
{
  return NO;
}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
  return NO;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
  UITableViewCell * cell = [self tableView:tableView cellForRowAtIndexPath:indexPath];
  OptionCell * option = (OptionCell *) cell;
  assert(option);
  
  CGFloat height = option.cellHeight;
  assert(height > 0);

  //NSLog(@"Cell {%d,%d} height is %f", indexPath.section, indexPath.row, height);

  return height;
}

/*
 To conform to Human Interface Guildelines, since selecting a row would have no effect (such as navigation), make sure that rows cannot be selected.
 */
- (NSIndexPath *)tableView:(UITableView *)tableView willSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	return nil;
}

@end




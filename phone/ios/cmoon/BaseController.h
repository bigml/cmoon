//
//  BaseController.h
//  cmoon
//
//  Created by li yajie on 12-7-23.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ArcHelper.h"
#import "DidaAppDelegate.h"


#define SCREEN_WIDTH [UIScreen mainScreen].bounds.size.width
#define SCREEN_HEIGHT [UIScreen mainScreen].bounds.size.height
#define DIDAAPP (DidaAppDelegate*)[UIApplication sharedApplication].delegate

@interface BaseController : UIViewController

@end

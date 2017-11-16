//
//  P2UXPeekViewController.h
//  P2UXApp
//
//  Copyright © 2017 Phase 2 Industries, LLC. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef NS_ENUM(NSInteger, P2UXPeekPosition) {
    P2UXPeekPosition_Left,
    P2UXPeekPosition_Right
};

@protocol P2UXPeekViewControllerDelegate
- (void) restoreComplete:(UIViewController*)controller;
@end;

@interface P2UXPeekViewController : UIViewController<UIGestureRecognizerDelegate>
@property (nonatomic, readonly) BOOL peek;
@property (nonatomic, weak) id<P2UXPeekViewControllerDelegate> delegate;

- (void) setCenterController:(UIViewController*)center;
- (void) addPeekController:(UIViewController*)controller;
- (void) peekView:(NSDictionary*)spec transition:(P2UXEventTransition*)transition;
- (void) restore:(BOOL)animated;

@end

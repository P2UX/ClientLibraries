//
//  P2UXAppNavigationController.h
//  P2UXApp
//
//  Created by Stephen Schalkhauser on 6/7/17.
//  Copyright © 2017 Phase 2 Industries, LLC. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface P2UXNavigationController : UINavigationController<UINavigationControllerDelegate>
@property (nonatomic) P2UXAppController* appController;

- (instancetype) initWithView:(P2UXDefinition*)def behavior:(P2UXAppBehavior*)behavior;
- (void) updateNavigation:(P2UXScreenNavigation*)navigation viewController:(UIViewController*)viewController animated:(BOOL)animated;
@end

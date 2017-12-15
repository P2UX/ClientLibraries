//
//  UIControl+P2UXControl.h
//  P2UXCore
//
//  Created by Stephen Schalkhauser on 3/23/15.
//  Copyright (c) 2015 Phase 2 Industries, LLC. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIControl (P2UX)
- (BOOL) setProperty:(NSString *)name withNumberValue:(NSNumber *)numberValue relative:(BOOL)relative animated:(id)animated itemspec:(NSDictionary *)itemspec;
- (id)   valueForAttribute:(NSString *)attribute;
- (void) setEnabledValue:(BOOL)enabled;
@end

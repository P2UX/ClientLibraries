//
//  P2UXTransitionFactory.h
//  P2UXCore
//
//  Created by Stephen Schalkhauser on 11/1/17.
//  Copyright © 2017 P2UX, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <P2UXCore/P2UXTransition.h>

@interface P2UXTransitionManager : NSObject
+ (instancetype) instance;

- (void) registerTransitionClass:(Class)transitionclass forName:(NSString*)name;
- (P2UXTransition*) createTransitionWithName:(NSString*)name spec:(NSDictionary*)spec;

@end

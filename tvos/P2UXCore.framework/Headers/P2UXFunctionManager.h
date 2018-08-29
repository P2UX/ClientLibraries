//
//  P2UXFunctionManager.h
//  P2UXCore
//
//  Created by Stephen Schalkhauser on 5/16/18.
//  Copyright © 2018 P2UX, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol P2UXFunctionHandler
- (id) handleFunction:(NSString*)funcName params:(NSArray*)params vars:(NSDictionary*)vars;
@end

@interface P2UXFunctionManager : NSObject
{
    NSMutableArray* _funcHandlers;
}
+ (instancetype) instance;
- (void) addFuncHandler:(id<P2UXFunctionHandler>)handler;
- (id) handleFunction:(NSString*)funcName params:(NSArray*)params vars:(NSDictionary*)vars;
@end

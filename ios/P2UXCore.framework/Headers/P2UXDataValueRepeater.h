//
//  P2UXDataValueRepeater.h
//  P2UXCore
//
//  Created by Stephen Schalkhauser on 10/26/17.
//  Copyright © 2017 P2UX, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface P2UXDataValueRepeater : NSObject

- (instancetype) init;
- (void) cleanup;
- (void) setVariableFromAction:(P2UXEventAction*)actiontohandle element:(P2UXElementInstance*)elem sender:(id)sender viewDelegate:(id<P2UXViewContainerDelegate>)viewDelegate;
- (void) updateRequestValueFromAction:(P2UXEventAction*)action element:(P2UXElementInstance*)elem sender:(id)sender viewDelegate:(id<P2UXViewContainerDelegate>)viewDelegate;

@end

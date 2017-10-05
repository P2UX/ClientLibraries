//
//  P2UXScrollbarDelegate.h
//  P2UXCore
//
//  Created by Stephen Schalkhauser on 8/17/17.
//  Copyright © 2017 P2UX, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol P2UXScrollbarDelegate <NSObject>
- (void) setScrollPos:(NSInteger)pos animated:(BOOL)animated;
- (void) endScrollPagingAnimated:(BOOL)animated;
@end

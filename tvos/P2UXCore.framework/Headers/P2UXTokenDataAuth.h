//
//  P2UXTokenDataAuth.h
//  P2UXCore
//
//  Created by Stephen Schalkhauser on 9/11/17.
//  Copyright © 2017 P2UX, Inc. All rights reserved.
//
#import "P2UXDataAuth.h"
#import "P2UXDataAuthSubclass.h"

@interface P2UXTokenDataAuth : P2UXDataAuth
{
@protected
    NSString*       _accessToken;
    NSString*       _refreshToken;
    NSDate*         _expires;
}
@property (readonly) BOOL expired;

- (BOOL) shouldRefresh;
@end

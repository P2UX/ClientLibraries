//
//  NSString+P2UX.h
//  P2UXCore
//
//  Created by Stephen Schalkhauser on 1/10/13.
//  Copyright (c) 2014 Phase 2 Industries, LLC. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSString (P2UX)
- (NSString *) URLEncodeString;
- (BOOL) isNumeric;
- (BOOL) isSVG;
- (BOOL) hasSubstring:(NSString*)substring;
- (id) numberOrString;
- (NSString*) zbase32String;
- (NSString*) base32String;
+ (NSString*) stringFromBase32String:(NSString *)base32String;
+ (NSString*) stringFromZbase32String:(NSString *)base32String;

@end

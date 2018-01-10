//
//  MQTTDataSource.h
//  MQTTMobile
//
//  Created by Stephen Schalkhauser on 12/9/17.
//  Copyright © 2017 P2UX, Inc. All rights reserved.
//

#import <P2UXCore/P2UXCore.h>
@import MQTTFramework;

@interface MQTTDataSource : P2UXDataSource<MQTTSessionDelegate>
+ (void) setup;
@end

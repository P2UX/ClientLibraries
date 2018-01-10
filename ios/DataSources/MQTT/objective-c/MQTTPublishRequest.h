//
//  MQTTPublishRequest.h
//  MQTTMobile
//
//  Created by Stephen Schalkhauser on 1/7/18.
//  Copyright © 2018 P2UX, Inc. All rights reserved.
//

#import "MQTTDataRequest.h"

@interface MQTTPublishRequest : MQTTDataRequest
@property (nonatomic) id data;
@property (nonatomic, readonly) BOOL retainData;

- (instancetype) initWithTopic:(NSString *)topic qos:(MQTTQosLevel)qos request:(NSString *)request callback:(P2UXCallbackResponse)callback data:(id)data retainData:(BOOL)retainData;
@end

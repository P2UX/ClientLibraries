//
//  MQTTPublishRequest.m
//  MQTTMobile
//
//  Created by Stephen Schalkhauser on 1/7/18.
//  Copyright © 2018 P2UX, Inc. All rights reserved.
//

#import "MQTTPublishRequest.h"

@implementation MQTTPublishRequest

- (instancetype) initWithTopic:(NSString *)topic qos:(MQTTQosLevel)qos request:(NSString *)request callback:(P2UXCallbackResponse)callback data:(id)data retainData:(BOOL)retainData
{
    self = [super initWithTopic:topic qos:qos request:request callback:callback];
    if (self != nil)
    {
        _data = data;
        _retainData = retainData;
    }
    return self;
}
@end

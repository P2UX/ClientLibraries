//
//  MQTTDataRequest.m
//  MQTTMobile
//
//  Created by Stephen Schalkhauser on 1/7/18.
//  Copyright © 2018 P2UX, Inc. All rights reserved.
//

#import "MQTTDataRequest.h"

@implementation MQTTDataRequest

- (instancetype) initWithTopic:(NSString*)topic qos:(MQTTQosLevel)qos request:(NSString*)request callback:(P2UXCallbackResponse)callback
{
    self = [super init];
    if (self != nil)
    {
        _callback = callback;
        _request = request;
        _topic = topic;
        _qos = qos;
    }
    return self;
}

- (void) cleanup
{
    _callback = nil;
    _request = nil;
    _topic = nil;
    _requestSpec = nil;
    _schemaSpec = nil;
    _args = nil;
}

- (void) setRequestInstanceData:(NSDictionary*)requestSpec schemaSpec:(NSDictionary*)schemaSpec args:(NSDictionary*)args 
{
    _requestSpec = requestSpec;
    _schemaSpec = schemaSpec;
    _args = args;
}

- (BOOL) hasRequestData
{
    return _requestSpec != nil;
}
@end

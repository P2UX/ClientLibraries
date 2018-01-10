//
//  MQTTDataRequest.h
//  MQTTMobile
//
//  Created by Stephen Schalkhauser on 1/7/18.
//  Copyright © 2018 P2UX, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
@import P2UXCore;
@import MQTTFramework;

@interface MQTTDataRequest : NSObject
@property (nonatomic, readonly) NSString* request;
@property (nonatomic, readonly) NSDictionary* requestSpec;
@property (nonatomic, readonly) NSDictionary* schemaSpec;
@property (nonatomic, readonly) NSDictionary* args;
@property (nonatomic, readonly) NSString* topic;
@property (nonatomic, readonly) MQTTQosLevel qos;
@property (nonatomic, readonly) P2UXCallbackResponse callback;
@property (nonatomic, readonly) BOOL hasRequestData;

- (instancetype) initWithTopic:(NSString*)topic qos:(MQTTQosLevel)qos request:(NSString*)request callback:(P2UXCallbackResponse)callback;
- (void) setRequestInstanceData:(NSDictionary*)requestSpec schemaSpec:(NSDictionary*)schemaSpec args:(NSDictionary*)args;
- (void) cleanup;
@end

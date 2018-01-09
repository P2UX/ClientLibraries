//
//  MQTTDataSourceWorkflow.m
//  MQTTMobile
//
//  Created by Stephen Schalkhauser on 12/12/17.
//  Copyright © 2017 P2UX, Inc. All rights reserved.
//
#import "MQTTDataSourceWorkflow.h"
@import P2UXApp;

NSString* const MQTTDataSourceWorkflow_Attrib_Topic = @"topic";

NSString* const MQTTDataSourceWorkflow_ArgFmt = @":%@";

@interface MQTTDataSourceWorkflow()
{
    NSDictionary        *_spec;
    NSMutableDictionary     *_topicCB;

}
@end

@implementation MQTTDataSourceWorkflow

- (instancetype) initWithSpec:(NSDictionary*)requestSpec
{
    self = [super init];
    if (self != nil)
    {
        _spec = requestSpec;
    }
    return self;
}

- (void) subscribeToTopic:(MQTTSession*)session args:(NSDictionary*)args callback:(P2UXCallbackResponse)callback
{
    if (_spec != nil)
    {
        NSString* topicSpec = [_spec objectForKey:MQTTDataSourceWorkflow_Attrib_Topic];
        topicSpec = [MQTTDataSourceWorkflow parameterizePath:topicSpec withArgs:args];
        if (![_topicCB objectForKey:topicSpec])
        {
            [_topicCB setObject:callback forKey:topicSpec];
            // initialize subscription for topic
            [session subscribeToTopic:topicSpec atLevel:2 subscribeHandler:^(NSError *error, NSArray<NSNumber *> *gQoss) {
                if (error) {
                    @synchronized(self)
                    {
                        [_topicCB removeObjectForKey:topicSpec];
                    }
                    NSLog(@"MQTTDataSource: Subscription failed %@", error.localizedDescription);
                } else {
                    NSLog(@"MQTTDataSource: Subscription sucessfull! Granted Qos: %@", gQoss);
                }
            }];
        }

    }
}

+ (NSString*) parameterizePath:(NSString*)path withArgs:(NSDictionary*)args
{
    if ([path containsString:@":"] && args != nil)
    {
        for (NSString* key in args.allKeys)
        {
            path = [path stringByReplacingOccurrencesOfString:[NSString stringWithFormat:MQTTDataSourceWorkflow_ArgFmt, key] withString:[args objectForKey:key]];
        }
    }
    return path;
}
@end

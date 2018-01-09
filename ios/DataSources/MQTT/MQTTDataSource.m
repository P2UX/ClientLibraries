//
//  MQTTDataSource.m
//  MQTTMobile
//
//  Copyright © 2017 P2UX, Inc. All rights reserved.
//
#import "MQTTDataSource.h"
#import "MQTTPublishRequest.h"

NSString* const MQTTDataSource_Type = @"MQTT";

NSString* const MQTTDataSource_Attrib_Host = @"host";
NSString* const MQTTDataSource_Attrib_Port = @"port";
NSString* const MQTTDataSource_Attrib_Timeout = @"timeout";

NSString* const MQTTDataSource_Workflow_Standard = @"std";
NSString* const MQTTDataSource_Workflow_AWSShadow = @"awsshadow";

NSString* const MQTTDataSource_RequestCommand_Subscribe = @"subscribe";
NSString* const MQTTDataSource_RequestCommand_Unsubscribe = @"unsubscribe";
NSString* const MQTTDataSource_RequestCommand_Publish = @"publish";
NSString* const MQTTDataSource_Command_Operator = @"operator";

NSString* const MQTTDataSource_RequestAttribute_Topic = @"topic";
NSString* const MQTTDataSource_RequestAttribute_Retain = @"retain";

int const MQTT_Default_Port = 1883;
NSTimeInterval const MQTT_Default_Timeout = 30;
int const MQTT_Default_QOS = MQTTQosLevelAtLeastOnce;

typedef NS_ENUM(NSInteger, MQTTConnectionState)
{
    MQTTConnectionState_Disconnected = 0,
    MQTTConnectionState_Connecting,
    MQTTConnectionState_Connected,
    MQTTConnectionState_Error
};

@interface MQTTDataSource()
{
    P2UXDataAuth            *_auth;
    NSString                *_host;
    int                      _port;
    MQTTQosLevel             _qos;
    MQTTCFSocketTransport   *_transport;
    MQTTSession             *_session;
    NSTimeInterval           _timeout;
    NSMutableArray          *_pendingSubscriptions;
    NSMutableArray          *_pendingPublishes;
    NSMutableDictionary     *_inprogressPublishes;
    NSMutableDictionary     *_subscriptions;
    MQTTConnectionState      _state;
    BOOL                     _runningPending;
}
@end

@implementation MQTTDataSource

+ (void) setup
{
    [[P2UXDataSourceManager instance] registerDataSourceClass:[MQTTDataSource class] forType:MQTTDataSource_Type];
}

// called for specific requests
-(id) initWithId:(NSString *)sourceId
            type:(NSString *)sourceType
         andSpec:(NSDictionary *)dataSpec
{
    self = [super initWithId:sourceId type:sourceType andSpec:dataSpec];
    if (self) {
        _runningPending = NO;
        _pendingPublishes = [NSMutableArray new];
        _pendingSubscriptions = [NSMutableArray new];
        _inprogressPublishes = [NSMutableDictionary new];
        _state = MQTTConnectionState_Disconnected;
        _timeout = [_initdata floatValueForKey:MQTTDataSource_Attrib_Timeout defaultValue:MQTT_Default_Timeout];
        _subscriptions = [NSMutableDictionary new];
        _port = MQTT_Default_Port;
        _requiresAsync = YES;
        _qos = MQTT_Default_QOS;
        _auth = nil;
        _host = [_initdata objectForKey:MQTTDataSource_Attrib_Host];
        _port = (int)[_initdata integerValueForKey:MQTTDataSource_Attrib_Port defaultValue:_port];
        
        // TODO: Implement Auth.
    }
    
    return self;
}

- (void) assertConnected
{
    @synchronized(self)
    {
        if (_state == MQTTConnectionState_Disconnected)
        {
            // try connecting
            if (_session == nil)
            {
                if (_transport == nil && _host != nil)
                {
                    _transport = [[MQTTCFSocketTransport alloc] init];
                    _transport.host = _host;
                    _transport.port = _port;
                }
                _session = [[MQTTSession alloc] init];
                _session.transport = _transport;
                _session.delegate = self;
            }
            NSLog(@"MQTTDataSource: Requesting connection to : %@ on port: %d", _host, _port);
            _state = MQTTConnectionState_Connecting;
            dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
                [_session connectAndWaitTimeout:_timeout];
            });
        }
    }
}

- (BOOL) authenticateForService:(NSString*)service withArgs:(NSDictionary*)args delegate:(id<P2UXDataSourceDelegate>)delegate
{
    
    if (self.authRequired && delegate != nil)
    {
        // TODO: Handle auth request
        // return YES;
    }
    return NO;
}

- (void) handleRequest:(NSString *)name withObject:(NSDictionary *)requestObj args:(NSDictionary *)args andCallback:(P2UXCallbackResponse)callback
{
    // Read the command operator if present,
    // default is to subscribe if the command is not supplied
    NSString* operator = MQTTDataSource_RequestCommand_Subscribe;
    if (args != nil)
    {
        NSDictionary* cmd = [args objectForKey:P2UXSpecKey_UCmd];
        if (cmd != nil)
        {
            operator = [cmd objectForKey:MQTTDataSource_Command_Operator];
            if (operator == nil)
            {
                operator = MQTTDataSource_RequestCommand_Subscribe;
            }
        }
    }

    // Get the details of the request so we can use it to respond
    // out of scope later when the async service requests complete
    NSString* topic = [requestObj objectForKey:MQTTDataSource_RequestAttribute_Topic];
    
    NSDictionary* schemaObj = nil;
    NSDictionary* requestSpec = [_requests objectForKey:name];
    if (requestSpec != nil)
    {
        schemaObj = [requestSpec objectForKey:P2UXSpecKey_Schema];
    }
    BOOL persist = NO;
    NSInteger validSecs = 0;
    NSInteger refresh = 0;
    NSDictionary* requestDetail = [requestSpec objectForKey:P2UXSpecKey_Request];
    if (requestDetail != nil)
    {
        NSDictionary* attrs = [requestDetail objectForKey:P2UXSpecKey_Attributes ];
        if (attrs != nil)
        {
            persist = [attrs boolValueForKey:P2UXSpecKey_Persist defaultValue:persist];
            validSecs = [attrs integerValueForKey:P2UXSpecKey_Validity defaultValue:validSecs];
            refresh = [attrs integerValueForKey:P2UXSpecKey_Refresh defaultValue:refresh]; 
        }
    }

    P2UXDataRequestResult response = -1;
    id resultData = nil;
    
    // Parameterize path with args
    if (topic != nil)
    {
        @synchronized(self)
        {
            if ([operator isEqualToString:MQTTDataSource_RequestCommand_Subscribe])
            {
                // Generic requests are treated as subscriptions to the request,
                // If the request topic already has a subscription, this will just return success
                // immediately
                response = P2UXRequestResult_Success;
                // see if there is already a subscription to the topic, if so, just return success, don't have to do anything...
                // the data should come from the persisted data or the next incoming message
                if ([_subscriptions objectForKey:name] == nil)
                {
                    MQTTDataRequest* subscription = [[MQTTDataRequest alloc] initWithTopic:topic qos:_qos request:name callback:callback];
                    [subscription setRequestInstanceData:requestSpec schemaSpec:schemaObj args:args];

                    if (_state == MQTTConnectionState_Disconnected)
                    {
                        // Not connected, stash the request as pending and don't return a response
                        // this will get cleared later when the subscription either fails or succeeds after
                        // the connection is established (or fails)
                        response = -1;
                        
                        // Avoid deadlock and make sure we aren't already running pending requests...
                        // This shouldn't happen, but make sure
                        if (!_runningPending)
                        {
                            @synchronized(_pendingSubscriptions)
                            {
                                [_pendingSubscriptions addObject:subscription];
                            }
                            // Start connection process
                            dispatch_async(dispatch_get_main_queue(), ^{
                                [self assertConnected];
                            });
                        }
                        else
                        {
                            NSLog(@"MQTTDataSource: Error trying to create a pending request while its already executing them");
                            response = P2UXRequestResult_Success;
                        }
                    }
                    else
                    {
                        [self addNewSubscription: subscription];
                    }
                }
                else
                {
                    // for this case, we just roll with local data since there is no explicit request
                    // model for MQTT. Whatever we have is whatever
                    NSLog(@"MQTTDataSource: Ignoring request for MQTT request: %@ for topic: %@ >> topic is already subscribed", name, topic);
                }
            }
            else if ([operator isEqualToString:MQTTDataSource_RequestCommand_Unsubscribe])
            {
                // unsubscribe to the topic for this request
                if ([_subscriptions objectForKey:topic] != nil)
                {
                    [self removeSubscription:topic];
                }
                else
                {
                    NSLog(@"Unsubscribe request made for a subscription that doesn't exist");
                    response = P2UXRequestResult_InvalidParams;
                }
            }
            else if ([operator isEqualToString:MQTTDataSource_RequestCommand_Publish])
            {
                // explicitly publish new data for the topic referenced by this request
                @try
                {
                    // don't send a response until the publish succeeds or fails
                    response = -1;
                    // Have to have valid data before trying to publish
                    id data = [args objectForKey:P2UXSpecKey_UData];
                    if (data != nil)
                    {
                        // validate data first...
                        P2UXDataSchema* schema = [self createSchemaWithSpec:schemaObj];
                        if ([schema validateData:data atContext:name])
                        {
                            BOOL retainData = [requestObj boolValueForKey:MQTTDataSource_RequestAttribute_Retain defaultValue:YES];
                            
                            MQTTPublishRequest* publishRequest = [[MQTTPublishRequest alloc] initWithTopic:topic qos:_qos request:name callback:callback data:data retainData:retainData];
                            [publishRequest setRequestInstanceData:requestObj schemaSpec:schemaObj args:args];
                            
                            if (_state == MQTTConnectionState_Disconnected)
                            {
                                if (!_runningPending)
                                {
                                    @synchronized (_pendingPublishes)
                                    {
                                        [_pendingPublishes addObject:publishRequest];
                                    }
                                    [self assertConnected];
                                }
                            }
                            else
                            {
                                [self publishData:publishRequest];
                            }
                        }
                        else
                        {
                            NSLog(@"MQTTDataSource: Publish data request made with data that doesn't match the data schema");
                            response = P2UXRequestResult_ValidationError;
                        }
                    }
                    else
                    {
                        NSLog(@"MQTTDataSource: Publish data request made without any data");
                        response = P2UXRequestResult_InvalidParams;
                    }
                }
                @catch (NSException* ex)
                {
                    NSLog(@"MQTTDataSource: Exception while trying to publish data: %@", ex);
                    response = P2UXRequestResult_Failed;
                }
            }
            else
            {
                // invalid command
                response = P2UXRequestResult_Failed;
            }
        }
    }
    else
    {
        // Specification is missing a topic, so it cannot continue
        NSLog(@"MQTTDataSource: No topic available for request");
        response = P2UXRequestResult_InvalidParams;
    }

    // if the response is something valid, callback to finish up the request
    // Otherwise, a response of -1 means that the response is pending and will be
    // resolved eventually.
    if ((NSInteger)response != -1)
    {
        callback(response, resultData);
    }
}

-(void)setResults:(P2UXSourcedData*)results
       forRequest:(NSString*)request
         withArgs:(NSDictionary*)args
       andPersist:(BOOL)persist
{
    [super setResults:results forRequest:request withArgs:args andPersist:persist];
    
    NSDictionary* requestSpec = [_requests objectForKey:request];
    if (!requestSpec) {
        return;
    }
    NSDictionary* requestObj = [requestSpec objectForKey:P2UXSpecKey_Request];
    if (!requestObj) {
        return;
    }
    
    NSString* topic = [requestObj objectForKey:MQTTDataSource_RequestAttribute_Topic];
    if (topic != nil)
    {
        id value = [results valueForKeyPath:@"."];
        if (value != nil)
        {
            BOOL retainData = [requestObj boolValueForKey:MQTTDataSource_RequestAttribute_Retain defaultValue:YES];
            MQTTPublishRequest* publishRequest = [[MQTTPublishRequest alloc] initWithTopic:topic qos:_qos request:request callback:nil data:value retainData:retainData];
            @synchronized(self)
            {
                if (_state == MQTTConnectionState_Disconnected)
                {
                    // Avoid deadlock and make sure we aren't already running pending requests...
                    // This shouldn't happen, but make sure
                    if (!_runningPending)
                    {
                        @synchronized(_pendingPublishes)
                        {
                            [_pendingPublishes addObject:publishRequest];
                        }
                        dispatch_async(dispatch_get_main_queue(), ^{
                            [self assertConnected];
                        });
                    }
                    else
                    {
                        NSLog(@"MQTTDataSource: Error trying to create a pending publish while its already executing them");
                    }
                }
                else
                {
                    // Already connected, so just publish the request
                    [self publishData:publishRequest];
                }
            }
        }
        else
        {
            NSLog(@"MQTTDataSource: setResults called with no data. Unable to update requested data link");
        }
    }
}

- (id) valueFromData:(NSData*)data
{
    if (data == nil) return nil;
    NSError* error;
    id value = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:&error];
    if (error != nil)
    {
        value = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    }
    return value;
}

- (NSData*) dataFromValue:(id)value
{
    NSData *data;
    if ([value isKindOfClass:[NSDictionary class]] || [value isKindOfClass:[NSArray class]])
    {
        // convert to JSON
        NSError* error;
        data = [NSJSONSerialization dataWithJSONObject:value options:0 error:&error];
        if (error != nil)
        {
            NSLog(@"MQTTDataSource: Unable to turn convert from JSON to NSData: %@", error);
        }
    }
    else
    {
        if ([value isKindOfClass:[NSNumber class]])
        {
            value = [value stringValue];
        }
        if ([value isKindOfClass:[NSString class]])
        {
            data = [value dataUsingEncoding:NSUTF8StringEncoding];
        }
    }
    return data;
}

- (void) clearPendingRequests
{
    @synchronized(_pendingSubscriptions)
    {
        for (MQTTDataRequest* subscription in _pendingSubscriptions)
        {
            [self cancelPendingRequest:subscription];
        }
        [_pendingSubscriptions removeAllObjects];
    }
    @synchronized(_pendingPublishes)
    {
        for (MQTTPublishRequest* publishReqeust in _pendingPublishes)
        {
            [self cancelPendingRequest:publishReqeust];
        }
        [_pendingPublishes removeAllObjects];
    }
}

- (void) cancelPendingRequest:(MQTTDataRequest*) request
{
    if (request == nil) return;
    // cancel the pending request by notifying the P2UXDataSource layer that the request
    // failed to complete
    request.callback(P2UXRequestResult_Failed, nil);
}

- (void) triggerPendingRequests
{
    if (_state == MQTTConnectionState_Connected)
    {
        _runningPending = YES;
        @synchronized(_pendingSubscriptions)
        {
            for (MQTTDataRequest* subscription in _pendingSubscriptions)
            {
                [self addNewSubscription:subscription];
            }
            [_pendingSubscriptions removeAllObjects];
        }
        @synchronized(_pendingPublishes)
        {
            for (MQTTPublishRequest* publishRequest in _pendingPublishes)
            {
                [self publishData:publishRequest];
            }
            [_pendingPublishes removeAllObjects];
        }
        _runningPending = NO;
    }
}

- (void) addNewSubscription:(MQTTDataRequest*)subscription
{
    if (subscription == nil) return;
    
    @synchronized(_subscriptions)
    {
        [_subscriptions setObject:subscription forKey:subscription.topic];
    }
    
    // initialize subscription for topic
    [_session subscribeToTopic:subscription.topic atLevel:subscription.qos subscribeHandler:^(NSError *error, NSArray<NSNumber *> *gQoss) {
        if (error) {
            @synchronized(_subscriptions)
            {
                [_subscriptions removeObjectForKey:subscription.topic];
                [subscription cleanup];
            }
            NSLog(@"MQTTDataSource: Subscription failed %@", error.localizedDescription);
        } else {
            NSLog(@"MQTTDataSource: Subscription sucessfull! Granted Qos: %@", gQoss);
        }
    }];
}

- (void) removeSubscription:(NSString*)topic
{
    if (topic == nil) return;
    
    @synchronized (_subscriptions)
    {
        MQTTDataRequest* subscription = [_subscriptions objectForKey:topic];
        if (subscription != nil)
        {
            [_subscriptions removeObjectForKey:topic];
                // unsubscribe here!
//                mMQTTClient.unsubscribe(topic);
            [subscription cleanup];
        }
    }

}

-(void) publishData:(MQTTPublishRequest*) publishRequest
{
    if (publishRequest == nil) return;
    
    // Publish the data associated with the publishRequest
    if (publishRequest.data != nil)
    {
        NSData* data = [self dataFromValue:publishRequest.data];
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            int msgId = [_session publishData:data onTopic:publishRequest.topic retain:publishRequest.retainData qos:publishRequest.qos];
            if (msgId == 0)
            {
                // the message was either dropped or the QOS is 0 in which case we won't know if it succeeds.
                // Don't keep the publishRequest around at this point
                // as we'll never get a callback via the delegate that it succeeded or not
                publishRequest.callback((_qos==MQTTQosLevelAtMostOnce)?P2UXRequestResult_Success:P2UXRequestResult_Failed, nil);
                [publishRequest cleanup];
            }
            else
            {
                // stash the publish till it can be resolved later as success or failure
                @synchronized(_inprogressPublishes)
                {
                    [_inprogressPublishes setObject:publishRequest forKey:[NSNumber numberWithInt:msgId]];
                }
            }
        });
    }
    else
    {
        // for this case, we just roll with local data since there is no explicit request
        // model for MQTT. Whatever we have is whatever
        NSLog(@"MQTTDataSource: Trying to publish new data for MQTT that is nil... topic: %@", publishRequest.topic);
        if (publishRequest.hasRequestData)
        {
            [self cancelPendingRequest:publishRequest];
            [publishRequest cleanup];
        }
    }
}

// MQTT Delegate methods
- (void) newMessage:(MQTTSession *)session data:(NSData *)data onTopic:(NSString *)topic qos:(MQTTQosLevel)qos retained:(BOOL)retained mid:(unsigned int)mid
{
    @try
    {
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            id value = [self valueFromData:data];
            if (value != nil)
            {
                MQTTDataRequest* subscription = [_subscriptions objectForKey:topic];
                if (subscription != nil)
                {
                    subscription.callback(P2UXRequestResult_Success, value);
                }
            }
        });
    }
    @catch(NSException* ex)
    {
        
    }
    NSLog(@"MQTTDataSource: Received new message: %@", data);
}

- (void) messageDelivered:(MQTTSession *)session msgID:(UInt16)msgID topic:(NSString *)topic data:(NSData *)data qos:(MQTTQosLevel)qos retainFlag:(BOOL)retainFlag
{
    // If this is an inprogress publish, then remove it from the queue
    NSNumber* num = [NSNumber numberWithInt:msgID];
    @synchronized(_inprogressPublishes)
    {
        MQTTPublishRequest* request = [_inprogressPublishes objectForKey:num];
        if (request != nil)
        {
            // If the request was triggered manually as a request, then notify the
            // callback that it has finished
            if (request.hasRequestData)
            {
                request.callback(P2UXRequestResult_Success, nil);
            }
            [_inprogressPublishes removeObjectForKey:num];
            [request cleanup];
        }
    }
}

- (void) connected:(MQTTSession *)session
{
    if (_session == session)
    {
        NSLog(@"MQTTDataSource: Connected");
        @synchronized(self)
        {
            _state = MQTTConnectionState_Connected;
        }
        [self triggerPendingRequests];
    }
}

- (void) connectionRefused:(MQTTSession *)session error:(NSError *)error
{
    if (_session == session)
    {
        @try
        {
            NSLog(@"MQTTDataSource: Connection refused: %@", error);
        }
        @catch(NSException* ex)
        {}
        @synchronized(self)
        {
            _state = MQTTConnectionState_Disconnected;
        }
        [self clearPendingRequests];
    }
}

- (void) connectionError:(MQTTSession *)session error:(NSError *)error
{
    if (_session == session)
    {
        @try
        {
            NSLog(@"MQTTDataSource: Connection error: %@", error);
        }
        @catch(NSException* ex)
        {}
        @synchronized(self)
        {
            _state = MQTTConnectionState_Disconnected;
        }
        [self clearPendingRequests];
    }
}

- (void) connectionClosed:(MQTTSession *)session
{
    if (_session == session)
    {
        NSLog(@"MQTTDataSource: Disconnected");
        @synchronized(self)
        {
            _state = MQTTConnectionState_Disconnected;
        }
        [self clearPendingRequests];
    }
}
@end

//
//  P2UXDeviceDataSource.m
//  Player
//
//  Created by Stephen Schalkhauser on 3/27/18.
//  Copyright © 2018 P2UX, Inc. All rights reserved.
//

#import "P2UXDeviceDataSource.h"
#import <CoreMotion/CoreMotion.h>

NSString* const P2UXDeviceData_Request_Accelerometer = @"accelerometer";
NSString* const P2UXDeviceData_Request_Gyroscope = @"gyroscope";
NSString* const P2UXDeviceData_Request_Magnetometer = @"magnetometer";
NSString* const P2UXDeviceData_Request_Cmd_Start = @"start";
NSString* const P2UXDeviceData_Request_Cmd_Stop = @"stop";
NSString* const P2UXDeviceData_Request_Cmd_Once = @"once";
NSString* const P2UXDeviceData_Request_Rate = @"rate";

CGFloat const P2UXDeviceData_DefaultRate = 1.0;

@interface P2UXDeviceDataSource()
{
    P2UXCallbackResponse    _accelerometerCB;
    P2UXCallbackResponse    _gyroCB;
    P2UXCallbackResponse    _magnetometerCB;
    CMMotionManager*        _motion;
    NSOperationQueue*       _motionQueue;
    BOOL                    _deviceMotionActive;
}
@end

@implementation P2UXDeviceDataSource
+ (void) setup
{
    [[P2UXDataSourceManager instance] registerDataSourceClass:[P2UXDeviceDataSource class] forType:@"DEVICEDATA"];
}

-(id) initWithId:(NSString *)sourceId
            type:(NSString *)sourceType
         andSpec:(NSDictionary *)dataSpec
{
    self = [super initWithId:sourceId type:sourceType andSpec:dataSpec];
    if (self != nil)
    {
        _deviceMotionActive = NO;
        _motion = [CMMotionManager new];
    }
    return self;
}

- (NSString*)serializeArgs:(NSDictionary*)args forRequest:(NSString*)request
{
    NSMutableDictionary* modargs;
    if (args != nil)
    {
        modargs = [args mutableCopy];
        // don't include the args in the serialize request
        [modargs removeObjectForKey:P2UXSpecKey_UCmd];
        [modargs removeObjectForKey:@"rate"];
    }
    return [P2UXDataSource serializeArgs:modargs forRequest:request];
}


- (void) handleRequest:(NSString *)name withObject:(NSDictionary *)requestObj args:(NSDictionary *)args andCallback:(P2UXCallbackResponse)callback
{
    if ([name isEqualToString:P2UXDeviceData_Request_Accelerometer])
    {
        [self handleAccelerometerWithArgs:args callback:callback];
    }
    else if ([name isEqualToString:P2UXDeviceData_Request_Gyroscope])
    {
        [self handleGyroscopeWithArgs:args callback:callback];
    }
    else if ([name isEqualToString:P2UXDeviceData_Request_Magnetometer])
    {
        [self handleMagnetometerWithArgs:args callback:callback];
    }
}

- (BOOL) assertMotionCapture:(CGFloat)rate
{
    @synchronized(_motion)
    {
        if (_deviceMotionActive) return YES;
        if (![_motion isDeviceMotionAvailable]) return NO;
        if (_motionQueue == nil)
        {
            P2UXCoreLogVerbose(@"DeviceData: Starting Accelerometer updates");

            _motionQueue = [NSOperationQueue new];
            _motionQueue.name = @"motionqueue";
            _motionQueue.maxConcurrentOperationCount = 1;
            _motion.deviceMotionUpdateInterval = rate;
        }
        _deviceMotionActive = YES;
        [_motion startDeviceMotionUpdatesToQueue:_motionQueue withHandler:^(CMDeviceMotion * _Nullable motion, NSError * _Nullable error) {
            if (error == nil && motion != nil)
            {
                if (_accelerometerCB != nil)
                {
                    NSDictionary* data = @{@"x":[NSNumber numberWithDouble:motion.userAcceleration.x], @"y":[NSNumber numberWithDouble:motion.userAcceleration.y], @"z":[NSNumber numberWithDouble:motion.userAcceleration.z]};
                    _accelerometerCB(P2UXRequestResult_Success, data);
                }
                if (_gyroCB != nil)
                {
                    NSDictionary* data = @{@"x":[NSNumber numberWithDouble:motion.rotationRate.x], @"y":[NSNumber numberWithDouble:motion.rotationRate.y], @"z":[NSNumber numberWithDouble:motion.rotationRate.z]};
                    _gyroCB(P2UXRequestResult_Success, data);
                }
                if (_magnetometerCB != nil)
                {
                    NSDictionary* data = @{@"x":[NSNumber numberWithDouble:motion.magneticField.field.x], @"y":[NSNumber numberWithDouble:motion.magneticField.field.y], @"z":[NSNumber numberWithDouble:motion.magneticField.field.z]};
                    _magnetometerCB(P2UXRequestResult_Success, data);
                }
            }
        }];
    }
    return YES;
}

- (void) motionCaptureStop
{
    if (_deviceMotionActive)
    {
        if (_accelerometerCB == nil && _gyroCB == nil && _magnetometerCB == nil)
        {
            _deviceMotionActive = NO;
            [_motion stopDeviceMotionUpdates];
        }
    }
}

- (void) handleAccelerometerWithArgs:(NSDictionary*)args callback:(P2UXCallbackResponse)callback
{
    @synchronized(self)
    {
        if (args != nil)
        {
            NSString* cmd = [args objectForKey:P2UXSpecKey_UCmd];
            if (cmd == nil || [cmd isEqualToString:P2UXDeviceData_Request_Cmd_Start])
            {
                if (_accelerometerCB == nil)
                {
                    // setup new polling for accelerometer
                    CGFloat rate = [args floatValueForKey:P2UXDeviceData_Request_Rate defaultValue:P2UXDeviceData_DefaultRate];
                    if ([self assertMotionCapture:rate])
                    {
                        _accelerometerCB = callback;
                    }
                    else
                    {
                        callback(P2UXRequestResult_InvalidRequest, nil);
                    }
                }
                else
                {
                    callback(P2UXRequestResult_Cancelled, nil);
                }
            }
            else if ([cmd isEqualToString:P2UXDeviceData_Request_Cmd_Once])
            {
                if ([_motion isDeviceMotionAvailable])
                {
                    if (_deviceMotionActive == NO)
                    {
                        [_motion startDeviceMotionUpdates];
                    }
                    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 0.167*NSEC_PER_SEC), kBGQueue, ^{
                        CMDeviceMotion* motion = _motion.deviceMotion;
                        if (motion != nil)
                        {
                            NSDictionary* data = @{@"x":[NSNumber numberWithDouble:motion.userAcceleration.x], @"y":[NSNumber numberWithDouble:motion.userAcceleration.y], @"z":[NSNumber numberWithDouble:motion.userAcceleration.z]};
                            callback(P2UXRequestResult_Success, data);
                        }
                        else
                        {
                            callback(P2UXRequestResult_Failed, nil);
                        }
                        if (_deviceMotionActive == NO)
                        {
                            [_motion stopDeviceMotionUpdates];
                        }
                    });
                }
                else
                {
                    callback(P2UXRequestResult_InvalidRequest, nil);
                }
            }
            else if ([cmd isEqualToString:P2UXDeviceData_Request_Cmd_Stop])
            {
                if (_accelerometerCB != nil)
                {
                    P2UXCoreLogVerbose(@"DeviceData: Stopping Accelerometer updates");
                    _accelerometerCB = nil;
                }
                [self motionCaptureStop];
                callback(P2UXRequestResult_Success, nil);
            }
        }
    }
}

- (void) handleGyroscopeWithArgs:(NSDictionary*)args callback:(P2UXCallbackResponse)callback
{
    @synchronized(self)
    {
        if (args != nil)
        {
            NSString* cmd = [args objectForKey:P2UXSpecKey_UCmd];
            if (cmd == nil || [cmd isEqualToString:P2UXDeviceData_Request_Cmd_Start])
            {
                if (_gyroCB == nil)
                {
                    // setup new polling for accelerometer
                    CGFloat rate = [args floatValueForKey:P2UXDeviceData_Request_Rate defaultValue:P2UXDeviceData_DefaultRate];
                    if ([self assertMotionCapture:rate])
                    {
                        _gyroCB = callback;
                    }
                    else
                    {
                        callback(P2UXRequestResult_InvalidRequest, nil);
                    }
                }
                else
                {
                    callback(P2UXRequestResult_Cancelled, nil);
                }
            }
            else if ([cmd isEqualToString:P2UXDeviceData_Request_Cmd_Once])
            {
                if ([_motion isDeviceMotionAvailable])
                {
                    if (_deviceMotionActive == NO)
                    {
                        [_motion startDeviceMotionUpdates];
                    }
                    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 0.167*NSEC_PER_SEC), kBGQueue, ^{
                        CMDeviceMotion* motion = _motion.deviceMotion;
                        if (motion != nil)
                        {
                            NSDictionary* data = @{@"x":[NSNumber numberWithDouble:motion.rotationRate.x], @"y":[NSNumber numberWithDouble:motion.rotationRate.y], @"z":[NSNumber numberWithDouble:motion.rotationRate.z]};
                            callback(P2UXRequestResult_Success, data);
                        }
                        else
                        {
                            callback(P2UXRequestResult_Failed, nil);
                        }
                        if (_deviceMotionActive == NO)
                        {
                            [_motion stopDeviceMotionUpdates];
                        }
                    });
                }
                else
                {
                    callback(P2UXRequestResult_InvalidRequest, nil);
                }
            }
            else if ([cmd isEqualToString:P2UXDeviceData_Request_Cmd_Stop])
            {
                if (_gyroCB != nil)
                {
                    P2UXCoreLogVerbose(@"DeviceData: Stopping Gyro updates");
                    _gyroCB = nil;
                }
                [self motionCaptureStop];
                callback(P2UXRequestResult_Success, nil);
            }
        }
    }
}


- (void) handleMagnetometerWithArgs:(NSDictionary*)args callback:(P2UXCallbackResponse)callback
{
    @synchronized(self)
    {
        if (args != nil)
        {
            NSString* cmd = [args objectForKey:P2UXSpecKey_UCmd];
            if (cmd == nil || [cmd isEqualToString:P2UXDeviceData_Request_Cmd_Start])
            {
                if (_magnetometerCB == nil)
                {
                    // setup new polling for accelerometer
                    CGFloat rate = [args floatValueForKey:P2UXDeviceData_Request_Rate defaultValue:P2UXDeviceData_DefaultRate];
                    if ([self assertMotionCapture:rate])
                    {
                        _magnetometerCB = callback;
                    }
                    else
                    {
                        callback(P2UXRequestResult_InvalidRequest, nil);
                    }
                }
                else
                {
                    callback(P2UXRequestResult_Cancelled, nil);
                }
            }
            else if ([cmd isEqualToString:P2UXDeviceData_Request_Cmd_Once])
            {
                if ([_motion isDeviceMotionAvailable])
                {
                    if (_deviceMotionActive == NO)
                    {
                        [_motion startDeviceMotionUpdates];
                    }
                    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 0.167*NSEC_PER_SEC), kBGQueue, ^{
                        CMDeviceMotion* motion = _motion.deviceMotion;
                        if (motion != nil)
                        {
                            NSDictionary* data = @{@"x":[NSNumber numberWithDouble:motion.magneticField.field.x], @"y":[NSNumber numberWithDouble:motion.magneticField.field.y], @"z":[NSNumber numberWithDouble:motion.magneticField.field.z]};
                            callback(P2UXRequestResult_Success, data);
                        }
                        else
                        {
                            callback(P2UXRequestResult_Failed, nil);
                        }
                        if (_deviceMotionActive == NO)
                        {
                            [_motion stopDeviceMotionUpdates];
                        }
                    });
                }
                else
                {
                    callback(P2UXRequestResult_InvalidRequest, nil);
                }
            }
            else if ([cmd isEqualToString:P2UXDeviceData_Request_Cmd_Stop])
            {
                if (_magnetometerCB != nil)
                {
                    P2UXCoreLogVerbose(@"DeviceData: Stopping Gyro updates");
                    _magnetometerCB = nil;
                }
                [self motionCaptureStop];
                callback(P2UXRequestResult_Success, nil);
            }
        }
    }
}
@end

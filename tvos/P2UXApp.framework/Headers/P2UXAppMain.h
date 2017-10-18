//
//  P2UXAppMain.h
//  P2UXApp
//
//  Created by Stephen Schalkhauser on 6/2/14.
//  Copyright (c) 2014 Phase 2 Industries, LLC. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <P2UXApp/P2UXAppLoader.h>
#import <P2UXCore/P2UXCoreComponent.h>
#ifdef LOCAL_NOTIFICATION_SUPPORT
#import <UserNotifications/UserNotifications.h>
#endif

@interface P2UXAppMain : P2UXAppLoader<P2UXCoreComponentDelegate>
{
    BOOL        _initialized;
}

+ (NSBundle*) frameworkBundle;
+ (instancetype) instance;

/*!
 * @discussion Startup the P2UXApp framework.
 * @param authCode A valid API key. If this parameter is not valid, the framework will not initialize
 * @param resources An array resources that describe how the content is delivered and updated for the app.
 * @param level The logging level to use. Valid values are P2UXCoreLogLevelError, P2UXCoreLogLevelDebug, P2UXCoreLogLevelInfo, P2UXCoreLogLevelVerbose, P2UXCoreLogLevelNone
 * @param allowNotifications Whether or not to allow notifications from the UI Builder server for live updates.
 * @warning Make sure to call this method in your AppDelegate before any other use of the P2UXApp framework
 */

- (void) initializeWithAuthCode:(NSString*)authCode resources:(NSArray*)resources logLevel:(P2UXCoreLogLevel)level allowNotifications:(BOOL)allowNotifications delegate:(id<P2UXAppLoaderDelegate>)delegate;

#ifdef LOCAL_NOTIFICATION_SUPPORT
// DEPRICATED! - (void) systemLocalNotificationReceived:(UNNotificationRequest*)notification;
//- (void) systemLocalNotificationReceived:(UILocalNotification*)notification;
#endif

- (BOOL) checkForUpdates:(BOOL)manual app:(NSString*)app;
- (void) listenForUpdates:(BOOL) listen;

- (NSString*) licenseStatusMessage;
@end

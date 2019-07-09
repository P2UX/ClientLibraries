//
//  P2UXAppController.h
//  P2UXApp
//
//  Copyright © 2015 Phase 2 Industries, LLC. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <P2UXCore/P2UXPanel.h>
#import <P2UXApp/P2UXPanelController.h>
#import <P2UXApp/P2UXPeekViewController.h>
#ifdef LOCAL_NOTIFICATION_SUPPORT
#import <UserNotifications/UserNotifications.h>
#endif

@class P2UXEventTransition, P2UXScreenNavigation;

extern NSString*  const _Nonnull UPDATE_CONTENTS_NOW;


/*!
 The Application controller
 @discussion Provides management of the screen workflow for a screen window, application level event handlers and application data. All screen/panel level event requests are funneled back through this object.
 */
@interface P2UXAppController : UIViewController<P2UXViewContainerDelegate, P2UXEventHandlerDelegate,
    UINavigationControllerDelegate,
    P2UXModalLayerDelegate,
    P2UXPanelDelegate,
#ifdef LOCAL_NOTIFICATION_SUPPORT
    P2UXPeekViewControllerDelegate,
    UNUserNotificationCenterDelegate>
#else
    P2UXPeekViewControllerDelegate>
#endif
{
    P2UXAppDefinition*          _def;
    NSMutableArray*             _eventHandlers;
    NSMutableArray*             _backStack;
    NSMutableDictionary*        _backStackTransitions;
    NSMutableDictionary*        _viewCache;
    NSMutableDictionary*        _visiblePanels;
}

/*!
 The current view controller
 */
@property (nonatomic, readonly) UIViewController* _Nonnull current;
/*!
 The home view controller
 */
@property (nonatomic, readonly) UIViewController* _Nonnull home;
/*!
 The current application behavior
 */
@property (nonatomic) P2UXAppBehavior* _Nonnull behavior;
/*!
 The form factor (screens and views) associated with this device screen
 */
@property (nonatomic, readonly) P2UXAppFormFactor* _Nonnull formFactor;
/**
 Indicates whether or not this is a primary device screen.
 */
@property (nonatomic) BOOL primary;
@property (nonatomic, readonly) P2UXScreenNavigation* _Nonnull headerNavigation;
@property (nonatomic) CGFloat scale;
#ifdef LOCAL_NOTIFICATION_SUPPORT
@property (nonatomic, readonly) UNAuthorizationOptions notifyOptions;
#endif
- (nonnull instancetype) initWithFrame:(CGRect)frame;
- (nonnull instancetype) initWithExplicitSize:(CGRect)frame;
- (nonnull instancetype) initWithFormFactor:(nonnull P2UXAppFormFactor*)ff def:(nonnull P2UXAppDefinition*)def;
- (nonnull instancetype) initWithFormFactor:(nonnull P2UXAppFormFactor*)ff def:(nonnull P2UXAppDefinition*)def size:(CGSize)size;

- (void) setFormFactor:(nonnull P2UXAppFormFactor *)ff andDef:(nonnull P2UXAppDefinition*)def;

- (void) cleanup;
- (void) reloadContents:(nonnull P2UXAppFormFactor*)ff andDef:(nonnull P2UXAppDefinition*)def;
- (void) setAppStateFromAction:(nonnull P2UXEventAction*)action;
- (void) setAppState:(nonnull NSString*)state;
- (nonnull NSString*) appState;

- (BOOL) handleExternalEvent:(nonnull NSString*)eventType params:(nullable id)params;

- (void) resetEvents;

- (void) presentViewController:(nonnull UIViewController *)viewControllerToPresent animated:(BOOL)flag completion:(nullable void (^)(void))completion;

#pragma mark - Navigation
- (nullable P2UXScreenNavigation*) headerNavigationWithName:(nonnull NSString*)name;

#pragma mark - Event handlers
- (void) handleEvents:(nonnull NSArray*)events element:(nullable P2UXElementInstance*)element source:(nullable id)source;

#pragma mark - Variable access
- (nullable id) varForKey:(nonnull NSString*)key;
- (BOOL) setVarValue:(nonnull id)value forKey:(nonnull NSString*)key usingIdent:(nullable NSString*)ident notify:(BOOL)notify itemspec:(nullable NSDictionary*)itemspec;

#pragma mark - Current View methods
- (BOOL) setCurrentView:(nonnull NSString*)viewId history:(P2UXEventBackHistory)history;
- (BOOL) setCurrentView:(nonnull NSString*)viewId history:(P2UXEventBackHistory)history transition:(nullable P2UXEventTransition*)transition;
- (BOOL) setCurrentView:(nonnull NSString*)viewId history:(P2UXEventBackHistory)history data:(nullable id)data transition:(nullable P2UXEventTransition*)transition toggle:(BOOL)toggle index:(nullable id)index rect:(nullable NSDictionary*)rect modal:(BOOL)modal;
- (void) clearCachedViews;

#pragma mark - Overlay methods
- (nullable P2UXPanel*) showPanel:(nonnull NSString*)ident timeout:(nullable NSNumber*)timeout show:(NSInteger)show pos:(nullable NSDictionary*)pos size:(nullable NSDictionary*)size modalColor:(nullable NSString*)modalColor transition:(nullable P2UXEventTransition*)transition params:(nullable NSDictionary*)params data:(nullable id)data;
- (void) showPanelFromAction:(nonnull P2UXEventAction*)action;
- (nullable P2UXPanelController*) visiblePanelWithIdent:(nonnull NSString*)ident;
- (void) hidePanel:(nonnull NSString*)systemType transition:(nullable P2UXEventTransition*)transition;
- (void) hidePanel:(nonnull NSString*)ident fade:(BOOL)fade;
- (void) hidePanelFromAction:(nonnull P2UXEventAction*)action;

#pragma mark - Modal Layer methods
- (void) createModalLayer:(nonnull UIView*)view color:(nullable NSString*)color fade:(float)fade;

#pragma mark - Back stack methods
- (void) handleBack;
- (void) clearBackStack;
- (void) handleBackToView:(nonnull NSString*)viewId transition:(nullable P2UXEventTransition*)transition;
- (void) addLayoutToBackStack:(nonnull UIView*)view layout:(nonnull NSString*)layout transient:(BOOL)transient duration:(float)duration replace:(BOOL)replace clear:(BOOL)clear;
- (void) removeLastOnBackStack;

#pragma mark - P2UXEventHandlerDelegate
- (void) eventsComplete:(nonnull id)handler;

#pragma mark - P2UXModalLayerDelegate
- (void) layerTap:(nonnull P2UXModalLayer *)layer;

#pragma mark - P2UXViewContainerDelegate
- (void) setValue:(nonnull id)value forLink:(nonnull P2UXVarLink *)link withIndex:(nullable id)index;
- (nullable P2UXDefinition*) screenDefWithIdentOrSystemType:(nonnull NSString*)ident;
- (nullable P2UXDefinition*) panelDefWithIdentOrSystemType:(nonnull NSString*)ident;
- (nullable P2UXView*) currentView;
- (nullable P2UXView*) cachedView:(nonnull NSString*) ident;
- (void) cacheView:(nonnull P2UXView*)view;
- (nonnull UIViewController*) currentController;
- (nullable NSDictionary*) nativeDialogWithIdent:(nonnull NSString*)ident;
- (CGRect)  containerBounds;
- (nullable OSColorClass*) colorWithId:(nonnull NSString*)colorWithId;
- (nullable NSDictionary*) gradientWithId:(nonnull NSString*)gradientWithId;
- (nonnull UIView*) overlayParent;
- (nullable P2UXView*) createViewItemWithDef:(nonnull P2UXDefinition*)def rect:(CGRect)rect cache:(BOOL)cache index:(nullable id)index data:(nullable id)data;
- (void) handleTimeUpdate;
- (nonnull id<P2UXHelperDelegate>) helperDelegate;
- (void) showOverlaysForCurrentView;
- (void) removeOverlay:(nonnull NSString*)removeOverlay;
- (void) peekScreen:(nonnull NSString*)ident spec:(nullable NSDictionary*)spec index:(nullable id)index data:(nullable id)data transition:(nullable P2UXEventTransition*)transition;
- (nullable NSArray*) eventsWithEventType:( P2UXElementEvent)eventType;

#pragma mark - P2UXPanelDelegate
- (void) viewDidClose:(nonnull id)view;

#ifdef LOCAL_NOTIFICATION_SUPPORT
- (void) handleSystemLocalNotification:(nonnull UNNotification*)notification;
- (void) handleRemoteNotificationRegistration:(nonnull NSData*)deviceToken;
- (void) handleRemoteNotificationFailure:(nonnull NSError*)error;
#endif

- (void) setupWithFormFactor:(nonnull P2UXAppFormFactor*)formFactor def:(nonnull P2UXAppDefinition*)def;
- (BOOL) handleBinaryUpdate:(nullable NSString*)version;

@end

//
//  P2UXEvent.h
//  P2UXCore
//
//  Created by Stephen Schalkhauser on 6/11/13.
//  Copyright (c) 2013  Phase 2 Industries, LLC. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <P2UXCore/P2UXEventAction.h>

@class P2UXEventTransition;
@class P2UXElementInstance;

extern NSString* const P2UXEvent_Global_Message;
extern NSString* const P2UXEvent_Message_Name;
extern NSString* const P2UXEvent_Message_Data;
extern NSString* const P2UXEvent_Message_Element;
extern NSString* const P2UXEvent_Message_Sender;
extern NSString* const P2UXEvent_Media_Message;
extern NSString* const P2UXEvent_Layout;
extern NSString* const P2UXEvent_Target_Success;
extern NSString* const P2UXEvent_Target_Fail;
extern NSString* const P2UXEvent_Global_Attribute_Changed;
extern NSString* const P2UXEvent_Attrib_Value;
extern NSString* const P2UXEvent_Attrib_Name;
extern NSString* const P2UXEvent_Attrib_Actions;
extern NSString* const P2UXEvent_Attrib_Event;

extern NSString* const P2UXEvent_INPUT_VALUE;
extern NSString* const P2UXEvent_INPUT_BACKSPACE;
extern NSString* const P2UXEvent_INPUT_SPACE;
extern NSString* const P2UXEvent_INPUT_CUSTOM;
extern NSString* const P2UXEvent_INPUT_CLEAR;
extern NSString* const P2UXEvent_INPUT_NEWLINE;

extern NSString* const P2UXEvent_UserAuth_Message;
extern NSString* const P2UXEvent_UserAuth_Action;
extern NSString* const P2UXEvent_UserAuth_Result;
extern NSString* const P2UXEvent_Fingerprint_Result;
extern NSString* const P2UXEvent_Attrib_Service;
extern NSString* const P2UXEvent_Attrib_Args;
extern NSString* const P2UXEvent_Attrib_Action;
extern NSString* const P2UXEvent_AuthenticateInput;
extern NSString* const P2UXEvent_AuthenticateComplete;
extern NSString* const P2UXEvent_AuthAction_Request;
extern NSString* const P2UXEvent_AuthAction_Cancel;
extern NSString* const P2UXEvent_AuthAction_Clear;

@interface P2UXEvent : NSObject
{
    NSMutableDictionary*    _linkActions;
    NSArray*                _actions;
}
@property (nonatomic) P2UXElementEvent event;
@property (nonatomic,readonly) NSDictionary* itemspec;
@property (nonatomic) NSDictionary* eventParams;
@property (nonatomic, weak) id eventContext;
@property (nonatomic, weak) id<P2UXViewContainerDelegate> viewDelegate;
@property (nonatomic) BOOL enabled;
@property (nonatomic) id data;
@property (nonatomic, readonly) BOOL sync;
@property (nonatomic, readonly) NSDictionary* conditionals;

+ (void) SendGlobalMessageEvent:(NSString*)message data:(id)data sender:(id)sender;
+ (void) NotifyAttributeChanged:(NSString*)attribute value:(id)value sender:(id)sender;
+ (void) NotifyFingerprintResult:(BOOL)result data:(id)data sender:(id)sender;
+ (NSArray*) dataEventsWithEventType:(P2UXElementEvent)eventType ident:(NSString*)ident request:(NSString*)request events:(NSArray*)events;
+ (NSArray*) dataEventsWithEventType:(P2UXElementEvent)eventType systemType:(NSString*)systemType request:(NSString*)request events:(NSArray*)events;
+ (NSArray*) dataEventsWithEventType:(P2UXElementEvent)eventType field:(NSString*)field value:(NSString*)value events:(NSArray*)events;

- (id) initWithEventSpec:(NSDictionary *)spec;
- (id) initWithActions:(NSArray*)actions itemspec:(NSDictionary*)itemspec;
- (void) cleanup;
- (void) clearLinks;

- (P2UXVarLinks*) links;

- (NSArray*) actions;
- (void) setActions:(NSArray*)actions;
- (NSArray*) actionsWithResult:(NSInteger)result context:(id)context;
- (id) itemSpecItem:(NSString*)itemName;

- (BOOL) isMessageEventNamed:(NSString*)message;
- (BOOL) isStateEventNamed:(NSString*)state;
- (BOOL) isFingerprintResultEqualTo:(BOOL)fingerprint;
- (BOOL) isAttributeEventForSender:(P2UXElementInstance*)elem named:(NSString*)named value:(id)value;
- (BOOL) isMediaAttributeEvent:(NSString*)attribute value:(id)value;
- (BOOL) isValueChangedEvent:(NSString*)msg;
- (BOOL) isService:(NSString*)service;
- (void) resetLinks;
- (id)   payloadValueForLink:(P2UXVarLink*)link;
- (NSDictionary*) varsWithContext:(id)context;
@end

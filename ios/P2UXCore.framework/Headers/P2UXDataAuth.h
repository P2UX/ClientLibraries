//
//  P2UXDataAuth.h
//  P2UXCore
//
//  Created by Steve Ansell on 1/2/15.
//  Copyright (c) 2015 Phase 2 Industries, LLC. All rights reserved.
//

#import <Foundation/Foundation.h>

@class P2UXAuthPersistData;

extern NSString* const P2UXDataAuth_Field_Username;
extern NSString* const P2UXDataAuth_Field_Password;
extern NSString* const P2UXDataAuth_Field_ClientId;
extern NSString* const P2UXDataAuth_Field_ClientSecret;
extern NSString* const P2UXDataAuth_Field_Pin;
extern NSString* const P2UXDataAuth_Field_Scope;
extern NSString* const P2UXDataAuth_Field_State;
extern NSString* const P2UXDataAuth_Field_RefreshToken;
extern NSString* const P2UXDataAuth_Field_Expires;

extern NSString* const P2UXAuthError_Unknown;
extern NSString* const P2UXAuthError_Params;
extern NSString* const P2UXAuthError_Network;
extern NSString* const P2UXAuthError_Cancel;

typedef NS_ENUM(NSInteger, P2UXAuthResult) {
    P2UXAuthResult_Success,
    P2UXAuthResult_Reserved,
    P2UXAuthResult_UnknownError,
    P2UXAuthResult_InvalidParams,
    P2UXAuthResult_Failed,
    P2UXAuthResult_NetworkError,
    P2UXAuthResult_Cancelled,
    P2UXAuthResult_Retry,
    P2UXAuthResult_RefreshFailed
};

typedef NS_ENUM(NSInteger, P2UXAuthType) {
    P2UXAuthType_None,
    P2UXAuthType_Basic,
    P2UXAuthType_Login,
    P2UXAuthType_OAuth
};

#define NUM_AUTH_TYPES P2UXAuthType_OAuth+1
#define JSON_TYPE @"application/json"

typedef void (^P2UXAuthParamResponse)(NSDictionary* params);
typedef void (^P2UXWebAuthResponse)(id response);

@protocol P2UXDataAuthDelegate <NSObject>

-(BOOL) authType:(P2UXAuthType)type forService:(NSString*)service requires:(NSDictionary*)params viaCallback:(P2UXAuthParamResponse)callback;
-(BOOL) authType:(P2UXAuthType)type forService:(NSString*)service returnedResult:(P2UXAuthResult)result onRequest:(NSString*)request fromSource:(id)source;
-(BOOL) authType:(P2UXAuthType)type forService:(NSString*)service webAuth:(NSURL*)authURL withRedirect:(NSString*)redirect andCallback:(P2UXWebAuthResponse)callback;

@end

typedef void (^P2UXAuthCallback)(P2UXAuthResult result, NSString* message);

@interface P2UXDataAuth : NSObject
{
    
@protected
    NSMutableDictionary* _pendingAuths;
    P2UXAuthType         _type;
    NSString*            _typeName;
    NSString*            _serviceName;
    NSString*            _method;
    NSURL*               _authURL;
    NSDictionary*        _defaultArgs;
    NSDictionary*        _headers;
    P2UXAuthPersistData* _persistData;
    NSString*            _persistItem;
}

@property (readonly) NSURL* authURL;
@property (readonly) NSString* serviceName;
@property (readonly) P2UXAuthType type;
@property (readonly) BOOL authorized;
@property (nonatomic, weak) id<P2UXDataAuthDelegate> delegate;
@property (nonatomic, copy) NSString* sourceDir;

+(P2UXDataAuth*)authRequest:(NSDictionary*)authSpec forURL:(NSURL*)URL schemaTypes:(NSDictionary*)schemaTypes;
- (id) initWithAuthSpec:(NSDictionary *)authSpec andURL:(NSURL*)URL schemaTypes:(NSDictionary*)schemaTypes;
- (void) initialize;
- (void)makeRequest:(NSString*)request fromSource:(id)source withOverrides:(NSDictionary *)authOverrides andCallback:(P2UXAuthCallback)callback;
- (void)authorizeRequest:(NSMutableURLRequest*)request forSession:(NSURLSession*)session;
- (void)dispatchPendingForRequest:(NSString*)request withResult:(P2UXAuthResult)result andMessage:(NSString*)message;
- (void)deauthorize;
- (NSDictionary*) appendToPayload:(NSDictionary*)payload method:(NSString*)method;
- (void) appendToHeaders:(NSMutableURLRequest*)request method:(NSString*)method;
- (NSMutableDictionary*) appendToParameters:(NSMutableDictionary*)parameters method:(NSString*)method;
- (NSString*) updatePathComponent:(NSString*)path method:(NSString*)method;
// Auth data persistance
- (void) persistResults:(NSDictionary*)results altPaths:(NSDictionary*)altPaths;
- (void) loadPersistResults;
- (NSString*) persistName;
-(NSArray*)clearPendingAuths;
- (P2UXSourcedData*) authData;
- (void) authRetry;
@end

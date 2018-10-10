//
//  P2UXDataSource.h
//  P2UXCore
//
//  Created by Steve Ansell on 6/13/14.
//  Copyright (c) 2014 Phase 2 Industries, LLC. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <P2UXCore/P2UXSourcedData.h>
#import "P2UXDataAuth.h"

#define DATA_ERR_REQUEST 200

#define REQ_ERR_AUTH       @"Failed authorization for request"
#define REQ_ERR_UNKNOWN    @"Unknown request error"
#define REQ_ERR_PARAMS     @"Invalid or missing parameters for request"
#define REQ_ERR_FAILED     @"The request failed to complete"
#define REQ_ERR_NETWORK    @"Network error occurred making request"
#define REQ_ERR_VALIDATION @"The request returned invalid or error results"
#define REQ_CANCELLED      @"The request was cancelled"

extern NSString* const P2UXSpecKey_SystemType;
extern NSString* const P2UXSpecKey_InitData;
extern NSString* const P2UXSpecKey_Requests;

extern NSString* const P2UXSpecKey_Request;
extern NSString* const P2UXSpecKey_Schema;
extern NSString* const P2UXSpecKey_Data;
extern NSString* const P2UXSpecKey_Date;
extern NSString* const P2UXSpecKey_Expires;
extern NSString* const P2UXSpecKey_Response;
extern NSString* const P2UXSpecKey_Args;
extern NSString* const P2UXSpecKey_Persist;
extern NSString* const P2UXSpecKey_Validity;
extern NSString* const P2UXSpecKey_Refresh;
extern NSString* const P2UXSpecKey_Object;
extern NSString* const P2UXSpecKey_Delegate;
extern NSString* const P2UXSpecKey_KeepAlive;
extern NSString* const P2UXSpecKey_Headers;
extern NSString* const P2UXSpecKey_Attributes;

extern NSString* const P2UXSpecKey_UKeys;

extern NSString* const P2UXSpecKey_Cmd;
extern NSString* const P2UXSpecKey_UCmd;
extern NSString* const P2UXSpecKey_UData;
extern NSString* const P2UXSpecKey_UIndex;
extern NSString* const P2UXSpecKey_UFile;

extern NSString* const P2UXCommand_Attrib_Operator;
extern NSString* const P2UXCommand_Attrib_Collection;

extern NSString* const P2UXCommandTypeString_Create;
extern NSString* const P2UXCommandTypeString_Update;
extern NSString* const P2UXCommandTypeString_Read;
extern NSString* const P2UXCommandTypeString_Delete;
extern NSString* const P2UXCommandTypeString_Patch;

typedef NS_ENUM(NSInteger, P2UXDataRequestResult) {
    P2UXRequestResult_Success,
    P2UXRequestResult_AuthError,
    P2UXRequestResult_UnknownError,
    P2UXRequestResult_InvalidParams,
    P2UXRequestResult_Failed,
    P2UXRequestResult_NetworkError,
    P2UXRequestResult_ValidationError,
    P2UXRequestResult_Retry,
    P2UXRequestResult_Update,
    P2UXRequestResult_Cancelled,
    P2UXRequestResult_Deleted,
    P2UXRequestResult_InvalidRequest
};

typedef NS_ENUM(NSInteger, P2UXDataCommandType) {
    P2UXCommandType_Read,
    P2UXCommandType_Update,
    P2UXCommandType_Create,
    P2UXCommandType_Delete,
    P2UXCommandType_Patch
};

extern NSString* const PERSIST_TYPE;

@class P2UXDataSource;

@protocol P2UXDataSourceDelegate <NSObject>

-(void) request:(NSString*)request
       withArgs:(NSDictionary*)args
 returnedResult:(P2UXDataRequestResult)result
    withResults:(P2UXSourcedData*)results
      forSource:(P2UXDataSource*)source;

-(void) request:(NSString*)request
       withArgs:(NSDictionary*)args
updatedProgress:(P2UXSourcedData*)progress
      forSource:(P2UXDataSource*)source;

-(id<P2UXDataAuthDelegate>)dataAuthHandler;
- (id) processRequestDataForSource:(NSString*)ident request:(NSString*)request data:(id)data;
@end

@interface P2UXDataSource : NSObject
{
@protected
    BOOL _requiresAsync;
    NSDictionary* _initdata;
    NSDictionary* _requests;
    NSMutableDictionary* _cachedResults;
    NSMutableDictionary* _cachedErrors;
    P2UXSourcedData* _lastError;
    __weak id<P2UXDataSourceDelegate> _delegate;
}

@property (readonly) NSString* type;
@property (readonly) NSString* id;
@property (readonly) NSString* systemType;
@property (readonly) BOOL authRequired;
@property (readonly) BOOL requiresAsync;
@property (readonly) BOOL requiresInternet;
@property (nonatomic, copy) NSString* sourceDir;

+ (P2UXDataCommandType)commandTypeFromString:(NSString*)commandString;

+ (NSString*)serializeArgs:(NSDictionary*)args forRequest:(NSString*)request;
+ (NSDictionary*) argsForSerialization:(NSDictionary*)args;

-(id) initWithSpec:(NSDictionary*)dataSpec;
-(id) initWithId:(NSString*)sourceId type:(NSString*)sourceType andSpec:(NSDictionary*)dataSpec;

- (void) cleanup;

-(BOOL) liveRequest:(NSString*)request;

-(BOOL) makeRequest:(NSString*)request
              async:(BOOL)async
           withArgs:(NSDictionary*)args
          keepAlive:(BOOL)keepAlive
        andDelegate:(id<P2UXDataSourceDelegate>)delegate;

-(P2UXSourcedData*) resultsForRequest:(NSString*)request withArgs:(NSDictionary*)args;
-(P2UXSourcedData*) resultsForRequest:(NSString*)request withArgs:(NSDictionary*)args makeCopy:(BOOL)copy;
-(P2UXSourcedData*) errorsForRequest:(NSString*)request withArgs:(NSDictionary*)args;
-(P2UXSourcedData*) authData;

-(void) authLogout;
-(void) cancelRequests;
-(void) cancelRequest:(NSString*)request withArgs:(NSDictionary*)args andDelegate:(id<P2UXDataSourceDelegate>)delegate;
-(BOOL) clearAllResults:(BOOL)includePersisted;
-(BOOL) clearResultsForRequest:(NSString*)request
                      withArgs:(NSDictionary*)args
              includePersisted:(BOOL)includePersisted;

-(void) setResults:(P2UXSourcedData*)results
        forRequest:(NSString*)request
          withArgs:(NSDictionary*)args
        andPersist:(BOOL)persist;

- (P2UXSourcedData*) createData:(id)data forRequest:(NSString*)request;

- (BOOL) clearAuthForService:(NSString*)service;
- (BOOL) authenticateForService:(NSString*)service withArgs:(NSDictionary*)args delegate:(id<P2UXDataSourceDelegate>)delegate;
- (NSString*)serializeArgs:(NSDictionary*)args forRequest:(NSString*)request;

// protected
-(P2UXSourcedData*) createData:(id)data andSchema:(P2UXDataSchema*)schema;
-(P2UXSourcedData*) createData:(id)data andSchema:(P2UXDataSchema*)schema cachedOn:(NSDate *)date;
-(P2UXDataSchema*) createSchemaWithSpec:(NSDictionary*)spec;
-(BOOL) persistRequest:(NSString*)request;
-(void) cacheResults:(id)results forRequest:(NSString*)request key:(NSString*)key;
-(BOOL) validateResultsForRequest:(NSString*)request args:(NSDictionary*)args;
@end

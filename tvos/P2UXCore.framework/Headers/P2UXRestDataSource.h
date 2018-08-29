//
//  P2UXRestDataSource.h
//  P2UXCore
//
//  Created by Steve Ansell on 6/13/14.
//  Copyright (c) 2014 Phase 2 Industries, LLC. All rights reserved.
//

#import "P2UXDataSourceSubclass.h"

extern NSString* const HTTPMethod_GET;
extern NSString* const HTTPMethod_PUT;
extern NSString* const HTTPMethod_POST;
extern NSString* const HTTPMethod_DELETE;
extern NSString* const HTTPMethod_PATCH;
extern NSString* const HTTPMethod_LIVE;
extern NSString* const HTTPMethod_CRUD;

@interface P2UXRestDownloadHandler : NSObject  <NSURLSessionDownloadDelegate>
-(id) initWithRequest:(NSString*)request
           serialized:(NSString*)reqWithArgs
             ofFormat:(NSString*)format
           andContent:(NSDictionary*)content
              onTasks:(NSMutableDictionary*)currTasks
         thenCallback:(P2UXCallbackResponse)callback;
@end

@interface P2UXRestUploadHandler: NSObject <NSURLSessionStreamDelegate>
-(id) initWithRequest:(NSString*)request
           serialized:(NSString*)reqWithArgs
             ofFormat:(NSString*)format
           andContent:(NSDictionary*)content
              onTasks:(NSMutableDictionary*)currTasks
         thenCallback:(P2UXCallbackResponse)callback;
@end

@interface P2UXRestDataSource : P2UXDataSource

@property (readonly) NSURL* baseURL;
@property (readonly) BOOL authRequired;
@property (readwrite) BOOL pushResponse;
@property (nonatomic) NSString* baseURLKey;
+ (NSString*) buildQuery:(NSString*)query withArgs:(NSMutableDictionary*)args;

-(BOOL) makeRequest:(NSString*)request
              async:(BOOL)async
           withArgs:(NSDictionary*)args
          keepAlive:(BOOL)keepAlive
        andDelegate:(id<P2UXDataSourceDelegate>)delegate;
-(void) request:(NSString*)request responsePushed:(id)response;

- (BOOL) authenticateForService:(NSString*)service withArgs:(NSDictionary*)args delegate:(id<P2UXDataSourceDelegate>)delegate;
@end

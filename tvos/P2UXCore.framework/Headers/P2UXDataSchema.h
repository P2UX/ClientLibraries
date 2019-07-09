//
//  P2UXDataParams.h
//  P2UXCore
//
//  Created by Steve Ansell on 1/6/15.
//  Copyright (c) 2015 Phase 2 Industries, LLC. All rights reserved.
//

#import <Foundation/Foundation.h>
@class P2UXDataSourceFilterRequest;

typedef NS_ENUM(NSInteger, P2UXSchemaType) {
    P2UXSchemaType_Unknown = 0,
    P2UXSchemaType_Object = (1 << 0),
    P2UXSchemaType_Array = (1 << 1),
    P2UXSchemaType_String = (1 << 2),
    P2UXSchemaType_Number = (1 << 3),
    P2UXSchemaType_Boolean = (1 << 4),
    P2UXSchemaType_Date = (1 << 5),
    P2UXSchemaType_ArrayOrObject = (1 << 6),
    P2UXSchemaType_Timestamp = (1<<7),
    P2UXSchemaType_TimestampMS = (1<<8)
};

extern NSString* const P2UXSchemaKey_Type;
extern NSString* const P2UXSchemaKey_Item;
extern NSString* const P2UXSchemaKey_Error;
extern NSString* const P2UXSchemaKey_Message;
extern NSString* const P2UXSchemaKey_Detail;
extern NSString* const P2UXSchemaKey_Total;
extern NSString* const P2UXSchemaKey_Current;
extern NSString* const P2UXSchemaKey_Unit;
extern NSString* const P2UXSchemaKey_UDefault;

@interface P2UXDataSchema : NSObject

@property (readonly)P2UXSchemaType type;
@property (readonly)NSDictionary* spec;
@property (retain, readwrite)id success;
@property (retain, readwrite)id failure;
@property (readonly)BOOL validationPending;
@property (readonly)BOOL failureIndicated;
@property (readonly)BOOL allowEmpty;
@property (nonatomic, weak) P2UXDataSourceFilterRequest* filterValue;
@property (readonly) id defaultValue;
@property (nonatomic) BOOL allowUnknown;
@property (nonatomic, weak) NSDictionary* schemaTypes;

+(P2UXSchemaType)stringToType:(NSString*)typeName;
+(P2UXDataSchema*)updateSchema;
+(P2UXDataSchema*)errorSchema;

-(id)initWithSpec:(NSDictionary*)schemaSpec schemaTypes:(NSDictionary*)schemaTypes;
-(NSArray*)keys;
-(NSDictionary*)requiredItems;
-(NSString*)aliasForKey:(NSString*)key;
-(P2UXDataSchema*)schemaForKey:(NSString*)key;
-(P2UXDataSchema*)schemaForPath:(NSString*)path;
-(P2UXDataSchema*)itemSchema;
-(NSString*)itemIndex;
-(BOOL)validateData:(id)dataValue atContext:(NSString*)context output:(BOOL)output;
-(id)filterType:(id)value atContext:(NSString*)context;
-(id)filterTypeOutput:(id)value atContext:(NSString*)context;
-(id)filterValue:(id)value atContext:(NSString*)context;
- (id) formatDataForOutput:(id)dataValue atContext:(NSString*)context;
- (id) applyDefaultValues:(id)dataValue atContext:(NSString*)context;

// Protected
-(P2UXDataSchema*)createSchemaWithSpec:(NSDictionary*)spec;

@end

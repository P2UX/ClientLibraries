//
//  P2UXLocalDataSource.h
//  P2UXCore
//
//  Created by Steve Ansell on 5/26/15.
//  Copyright (c) 2015 Phase 2 Industries, LLC. All rights reserved.
//

#import <P2UXCore/P2UXCore.h>
#import "P2UXDataSourceSubclass.h"

typedef NS_ENUM(NSInteger, P2UXDataUpdateType) {
    P2UXUpdateType_Set,
    P2UXUpdateType_Insert,
    P2UXUpdateType_Append,
    P2UXUpdateType_Remove,
    P2UXUpdateType_Merge
} ;

@interface P2UXLocalDataSource : P2UXDataSource

@end

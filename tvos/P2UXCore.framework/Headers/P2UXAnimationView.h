//
//  P2UXAnimationView.h
//  P2UXCore
//
//  Created by Stephen Schalkhauser on 10/23/18.
//  Copyright © 2018 P2UX, Inc. All rights reserved.
//

#import <P2UXCore/P2UXElement.h>
#import <P2UXCore/P2UXControl.h>
#import <P2UXCore/P2UXActionDelegate.h>
#import <P2UXCore/LOTAnimationView.h>

NS_ASSUME_NONNULL_BEGIN

@interface P2UXAnimationView : LOTAnimationView
@property (nonatomic) BOOL hideOnComplete;
- (id) initWithFrame:(CGRect)frame elemInst:(P2UXElementInstance *)elemInst uipath:(NSString *)path ext:(BOOL)ext viewDelegate:(id<P2UXViewContainerDelegate>)viewDelegate index:(id)index;

@end

NS_ASSUME_NONNULL_END

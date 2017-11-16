//
//  P2UXTransition.h
//  P2UXCore
//
//  Created by Stephen Schalkhauser on 11/1/17.
//  Copyright © 2017 P2UX, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
typedef NS_ENUM(NSInteger, P2UXTransitionEasing) {
    P2UXTransitionEasing_None = 0,
    P2UXTransitionEasing_In,
    P2UXTransitionEasing_Out,
    P2UXTransitionEasing_InOut
};

typedef NS_ENUM(NSInteger, P2UXTransitionScreen)
{
    P2UXTransitionScreen_Incoming,
    P2UXTransitionScreen_Outgoing
};

extern NSString* const P2UXTransition_Attrib_Duration;
extern NSString* const P2UXTransition_Attrib_Easing;
extern NSString* const P2UXTransition_Attrib_Type;

extern NSString* const P2UXTransition_Bounces_Low;
extern NSString* const P2UXTransition_Bounces_Medium;
extern NSString* const P2UXTransition_Bounces_High;
extern NSString* const P2UXTransition_Bounces_Custom;

@interface P2UXTransition : NSObject
{
    P2UXTransitionScreen _screen;
}
@property (nonatomic) CGFloat duration;
@property (nonatomic) CGFloat delay;
@property (nonatomic) P2UXTransitionEasing easing;
@property (nonatomic) UIViewAnimationOptions options;
@property (nonatomic, readonly) NSString* name;
@property (nonatomic, readonly) NSString* bounces;
@property (nonatomic, readonly) NSString* curveType;
@property (nonatomic, readonly) NSDictionary* controls;
@property (nonatomic, readonly) NSDictionary* itemSpec;
@property (nonatomic) BOOL valid;
@property (nonatomic) BOOL isFullScreen;
@property (nonatomic, readonly) P2UXTransitionScreen screen;
@property BOOL reverse;

+ (UIViewAnimationOptions) optionsFromEasing:(P2UXTransitionEasing)easing;

- (instancetype) initWithSpec:(NSDictionary*)spec;
- (void) parse:(NSDictionary*)spec;
- (BOOL) prepareTransitionWithView:(UIView*)view outgoing:(UIView*)outgoing data:(id)data viewIsOutgoing:(BOOL)viewIsOutgoing;
- (void) applyTransitionToView:(UIView*)view outgoing:(UIView*)outgoing;
- (void) applyTransitionToAttribute:(UICollectionViewLayoutAttributes*)attr parentBounds:(CGRect)parentBounds;
- (void) transitionComplete:(UIView*)view outgoing:(UIView*)outgoing complete:(BOOL)complete;

// helpers
- (void) assertZOrderForView:(UIView*)incoming outgoing:(UIView*)outgoing;
- (void) assertReverseZOrderForView:(UIView*)incoming outgoing:(UIView*)outgoing;
@end

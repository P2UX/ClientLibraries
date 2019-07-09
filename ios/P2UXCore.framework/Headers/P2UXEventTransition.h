//
//  P2UXEventTransition.h
//  P2UXCore
//
//  Created by Stephen Schalkhauser on 11/25/13.
//  Copyright (c) 2013 Phase 2 Industries, LLC. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <P2UXCore/easing.h>
#import <P2UXCore/P2UXTransition.h>

extern NSString* const P2UXEventTransition_FadeIn;
extern NSString* const P2UXEventTransition_FadeOut;
extern NSString* const P2UXEventTransition_Slide;
extern NSString* const P2UXEventTransition_Frame;
extern NSString* const P2UXEventTransition_State;
extern NSString* const P2UXEventTransition_Layout;
extern NSString* const P2UXEventTransition_Reveal;
extern NSString* const P2UXEventTransition_Zoom;
extern NSString* const P2UXEventTransition_Default;
extern NSString* const P2UXEventTransition_Parallax;
extern NSString* const P2UXEventTransition_Peek;

extern NSString* const P2UXEventTransition_BackHistory;
extern NSString* const P2UXEventTransition_Attrib_Items;
extern NSString* const P2UXEventTransition_Attrib_OutItems;
extern NSString* const P2UXEventTransition_Attrib_Item;
extern NSString* const P2UXEventTransition_Attrib_OutItem;
extern NSString* const P2UXEventTransition_Attrib_Transition;
extern NSString* const P2UXEventTransition_Attrib_Easing;

typedef NS_ENUM(NSInteger, P2UXTransitionAttribute) {
    kP2UXTransitionAttribute_frame = 0,
    kP2UXTransitionAttribute_alpha
};

typedef NS_ENUM(NSInteger,P2UXTransitionSequence) {
    kP2UXTransitionSequence_Parallel = 0,
    kP2UXTransitionSequence_Serial
};

typedef NS_ENUM(NSInteger, P2UXTransitionBackHistory) {
    kP2UXTransitionBackHistory_No = 0,
    kP2UXTransitionBackHistory_Yes
};

@class P2UXEventTransition;

@protocol P2UXEventTransitionDelegate <NSObject>
- (void) transitionComplete:(OSViewClass *)view outgoing:(OSViewClass*)outgoing transition:(P2UXEventTransition*)transition;
- (void) transitionReverseComplete:(OSViewClass*)view outgoing:(OSViewClass*)outgoing transition:(P2UXEventTransition*)transition;
@end

typedef void (^TransitionComplete)(UIView* view, BOOL reverse);


@interface P2UXEventTransition : NSObject
{
    P2UXTransitionSequence  _sequence;
    NSMutableArray*         _delegates;
}
@property (nonatomic, readonly) NSDictionary* desc;
@property (nonatomic, weak)     OSViewClass* source;
@property (nonatomic)           BOOL reverse;
@property (nonatomic)           BOOL outgoing;
@property (nonatomic, readonly) CGFloat duration;
@property (nonatomic)           id data;
@property (nonatomic, readonly) P2UXTransitionBackHistory backHistory;
@property (nonatomic) BOOL      below;
@property (nonatomic) NSDictionary* rect;
@property (nonatomic) BOOL      valid;
@property (nonatomic) BOOL      isDefault;
@property (nonatomic) BOOL      isFullScreen;
@property (nonatomic) BOOL      overrideReverse;
@property (nonatomic) P2UXTransitionSide side;
@property (nonatomic, readonly) P2UXTransitionEasing easing;

+ (AHEasingFunction) easing:(NSDictionary*)transition;
+ (P2UXEventTransition*) transitionForLayout:(NSString*)layout duration:(float)duration easing:(P2UXTransitionEasing)easing;

- (id)   initWithDictionary:(NSDictionary*)desc;
- (void) cleanup;

- (void) apply:(OSViewClass *)view outgoing:(OSViewClass*)outgoing data:(id)data;
- (void) apply:(OSViewClass *)view outgoing:(OSViewClass*)outgoing data:(id)data completion:(TransitionComplete)completion;
- (void) addDelegate:(__weak id<P2UXEventTransitionDelegate>)delegate;
- (void) removeDelegate:(id<P2UXEventTransitionDelegate>)delegate;
- (void) applyTransitionsToAttribute:(UICollectionViewLayoutAttributes*)attr parentBounds:(CGRect)parentBounds;
- (void) animateWithAnimations:(void (^)(void))animations completion:(void (^)(BOOL finished))completion;
- (void) animateWithDuration:(CGFloat)duration delay:(CGFloat)delay bounces:(NSString*)bounces options:(UIViewAnimationOptions)options animations:(void (^)(void))animations completion:(void (^)(BOOL finished))completion;
- (void) animateWithDuration:(CGFloat)duration options:(UIViewAnimationOptions)options animations:(void (^)(void))animations completion:(void (^)(BOOL finished))completion;
- (UIView*) primaryScreen:(UIView*)incoming outgoing:(UIView*)outgoing;
@end

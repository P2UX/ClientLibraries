//
//  P2UXRadioButtonGroup.h
//  P2UXCore
//
//  Created by Stephen Schalkhauser on 8/28/17.
//  Copyright © 2017 P2UX, Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <P2UXCore/P2UXActionDelegate.h>
#import <P2UXCore/P2UXViewContainerDelegate.h>

@class P2UXScreenStateDefinition;

typedef NS_ENUM(NSInteger, P2UXRadioButtonGroup_DataType) {
    P2UXRadioButtonGroup_DataType_Dynamic = 0,
    P2UXRadioButtonGroup_DataType_Static
};

@interface P2UXRadioButtonGroup : UIScrollView<UIScrollViewDelegate, P2UXActionDelegate>
{
    P2UXScrollStyle                   _scrollStyle;
    __weak id<P2UXActionDelegate>     _handler;
    BOOL                              _paging;
    BOOL                              _showScroll;
    NSInteger                         _initialItem;
    NSInteger                         _page;
    P2UXRadioButtonGroup_DataType     _dataType;
    __weak P2UXScreenStateDefinition* _parentDef;
    NSMutableArray*                   _items;
}
@property (nonatomic, readonly) NSString* type;
@property (nonatomic) NSInteger selected;
@property (nonatomic) BOOL      enabled;

- (id) initWithFrame:(CGRect)frame elemInst:(P2UXElementInstance *)elemInst uipath:(NSString *)path ext:(BOOL)ext handler:(id<P2UXActionDelegate>)handler parentDef:(P2UXScreenStateDefinition*)parentDef viewDelegate:(id<P2UXViewContainerDelegate>)viewDelegate index:(id)index;
@end

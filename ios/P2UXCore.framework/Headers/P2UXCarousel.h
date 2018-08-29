//
//  P2UXCarousel.h
//  P2UXCore
//
//  Created by Stephen Schalkhauser on 1/21/16.
//  Copyright © 2016 Phase 2 Industries, LLC. All rights reserved.
//

#import <P2UXCore/iCarousel.h>

#import <P2UXCore/P2UXActionDelegate.h>
#import <P2UXCore/P2UXBaseCtrl.h>
#import <P2UXCore/P2UXControl.h>
@class P2UXElementInstance, P2UXContainerElement, P2UXCollectionViewDataSource;

extern NSString* const CarouselView_Attribute_Content;
extern NSString* const CarouselView_Attribute_Spacing;
extern NSString* const CarouselView_Attribute_Size;
extern NSString* const CarouselView_Attribute_ItemSize;
extern NSString* const CarouselView_Attribute_ScrollStyle;
extern NSString* const CarouselView_Attribute_VisibleItems;
extern NSString* const CarouselView_Attribute_Views;
extern NSString* const CarouselView_View_Link;
extern NSString* const CarouselView_Attribute_SelectedIndex;
extern NSString* const CarouselView_Attribute_Bounces;
extern NSString* const CarouselView_Attribute_Wrap;

@interface P2UXCarousel : iCarousel<iCarouselDelegate, iCarouselDataSource>
{
    P2UXContainerElement*         _elem;

}
@property (nonatomic) P2UXCollectionViewDataSource* data;
- (id)initWithFrame:(CGRect)frame elemInst:(P2UXElementInstance*)elemInst uipath:(NSString *)path ext:(BOOL)ext handler:(id<P2UXActionDelegate>)handler viewDelegate:(id<P2UXViewContainerDelegate>)viewDelegate index:(id)index;
- (void) processData:(id)value;
- (id)   objectAtIndex:(NSInteger)index;
- (NSInteger) viewTemplateIndexForRow:(NSInteger)row;
- (id)   dataValueForRow:(NSInteger)row;
@end

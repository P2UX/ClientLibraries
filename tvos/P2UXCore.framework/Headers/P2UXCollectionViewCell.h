//
//  P2UXCollectionViewCell.h
//  P2UXCore
//
//  Created by Stephen Schalkhauser on 1/5/16.
//  Copyright © 2016 Phase 2 Industries, LLC. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <P2UXCore/P2UXActionDelegate.h>
#import <P2UXCore/P2UXPanel.h>
@class P2UXElement;
@class P2UXCollectionView;
@class P2UXCollectionViewDataSource;

@interface P2UXCollectionViewCell : UICollectionViewCell<P2UXPanelDelegate>
@property (nonatomic) id viewData;
@property (nonatomic, weak) id<P2UXActionDelegate> actionHandler;
@property (nonatomic) BOOL stfWidth;
@property (nonatomic) BOOL stfHeight;
@property (nonatomic, weak) P2UXCollectionViewDataSource* datasource;

- (void) setContents:(NSString*)viewIdent viewDelegate:(id<P2UXViewContainerDelegate>)viewDelegate index:(id)index data:(id)data;
- (void) setContentsFromSpec:(NSDictionary*)viewSpec ident:(NSString*)viewIdent path:(NSString*)path viewDelegate:(id<P2UXViewContainerDelegate>)viewDelegate index:(id)index data:(id)data;
- (void) setContentsFromDef:(P2UXDefinition*)viewDef path:(NSString*)path viewDelegate:(id<P2UXViewContainerDelegate>)viewDelegate index:(id)index data:(id)data;
- (void) setupReorderHandler:(P2UXCollectionView*)parent;
- (void) removeReorderHandler;
- (BOOL) sizeToFitWidth;
- (BOOL) sizeToFitHeight;
@end

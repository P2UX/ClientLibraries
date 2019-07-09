//
//  SDWebImage.h
//  P2UXCore
//
//  Created by Stephen Schalkhauser on 7/5/19.
//  Copyright © 2019 P2UX, Inc. All rights reserved.
//

#ifndef SDWebImage_h
#define SDWebImage_h

#import <P2UXCore/SDWebImageCompat.h>

#if SD_UIKIT
#import <UIKit/UIKit.h>
#endif

//! Project version number for WebImage.
FOUNDATION_EXPORT double WebImageVersionNumber;

//! Project version string for WebImage.
FOUNDATION_EXPORT const unsigned char WebImageVersionString[];

// In this header, you should import all the public headers of your framework using statements like #import <WebImage/PublicHeader.h>

#import <P2UXCore/SDWebImageManager.h>
#import <P2UXCore/SDWebImageCacheKeyFilter.h>
#import <P2UXCore/SDWebImageCacheSerializer.h>
#import <P2UXCore/SDImageCacheConfig.h>
#import <P2UXCore/SDImageCache.h>
#import <P2UXCore/SDMemoryCache.h>
#import <P2UXCore/SDDiskCache.h>
#import <P2UXCore/SDImageCacheDefine.h>
#import <P2UXCore/SDImageCachesManager.h>
#import <P2UXCore/UIView+WebCache.h>
#import <P2UXCore/UIImageView+WebCache.h>
#import <P2UXCore/UIImageView+HighlightedWebCache.h>
#import <P2UXCore/SDWebImageDownloaderConfig.h>
#import <P2UXCore/SDWebImageDownloaderOperation.h>
#import <P2UXCore/SDWebImageDownloaderRequestModifier.h>
#import <P2UXCore/SDImageLoader.h>
#import <P2UXCore/SDImageLoadersManager.h>
#import <P2UXCore/UIButton+WebCache.h>
#import <P2UXCore/SDWebImagePrefetcher.h>
#import <P2UXCore/UIView+WebCacheOperation.h>
#import <P2UXCore/UIImage+Metadata.h>
#import <P2UXCore/UIImage+MultiFormat.h>
#import <P2UXCore/UIImage+MemoryCacheCost.h>
#import <P2UXCore/SDWebImageOperation.h>
#import <P2UXCore/SDWebImageDownloader.h>
#import <P2UXCore/SDWebImageTransition.h>
#import <P2UXCore/SDWebImageIndicator.h>
#import <P2UXCore/SDImageTransformer.h>
#import <P2UXCore/UIImage+Transform.h>
#import <P2UXCore/SDAnimatedImage.h>
#import <P2UXCore/SDAnimatedImageView.h>
#import <P2UXCore/SDAnimatedImageView+WebCache.h>
#import <P2UXCore/SDImageCodersManager.h>
#import <P2UXCore/SDImageCoder.h>
#import <P2UXCore/SDImageAPNGCoder.h>
#import <P2UXCore/SDImageGIFCoder.h>
#import <P2UXCore/SDImageIOCoder.h>
#import <P2UXCore/SDImageFrame.h>
#import <P2UXCore/SDImageCoderHelper.h>
#import <P2UXCore/SDImageGraphics.h>
#import <P2UXCore/UIImage+GIF.h>
#import <P2UXCore/UIImage+ForceDecode.h>
#import <P2UXCore/NSData+ImageContentType.h>
#import <P2UXCore/SDWebImageDefine.h>
#import <P2UXCore/SDWebImageError.h>
#import <P2UXCore/SDWebImageOptionsProcessor.h>

// Mac
#if __has_include(<P2UXCore/NSImage+Compatibility.h>)
#import <P2UXCore/NSImage+Compatibility.h>
#endif
#if __has_include(<P2UXCore/NSButton+WebCache.h>)
#import <P2UXCore/NSButton+WebCache.h>
#endif
#if __has_include(<P2UXCore/SDAnimatedImageRep.h>)
#import <P2UXCore/SDAnimatedImageRep.h>
#endif

// MapKit
#if __has_include(<P2UXCore/MKAnnotationView+WebCache.h>)
#import <P2UXCore/MKAnnotationView+WebCache.h>
#endif

#endif /* SDWebImage_h */

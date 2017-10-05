//
//  P2UXTextToSpeech.h
//  P2UXCore
//
//  Created by Stephen Schalkhauser on 9/21/17.
//  Copyright © 2017 P2UX, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

@interface P2UXTextToSpeech : NSObject<AVSpeechSynthesizerDelegate>
+ (instancetype) instance;

- (void) speakText:(NSString*)text;
- (void) cancelSpeak;

@end

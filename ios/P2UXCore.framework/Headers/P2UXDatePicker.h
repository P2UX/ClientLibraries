//
//  P2UXDatePicker.h
//  P2UXCore
//
//  Created by Stephen Schalkhauser on 2/12/18.
//  Copyright © 2018 P2UX, Inc. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef NS_ENUM(NSInteger, P2UXDatePickerMode)
{
    P2UXDatePickerMode_DateTime = 0,
    P2UXDatePickerMode_Date,
    P2UXDatePickerMode_Time
};

typedef NS_ENUM(NSInteger, P2UXDatePickerValueFmt)
{
    P2UXDatePickerValueFmt_Seconds = 0,
    P2UXDatePickerValueFmt_MS,
    P2UXDatePickerValueFmt_StringFmt
};

@interface P2UXDatePicker : UIDatePicker
{
    P2UXDatePickerValueFmt  _format;
    NSString*               _valueFmtString;
}

@end

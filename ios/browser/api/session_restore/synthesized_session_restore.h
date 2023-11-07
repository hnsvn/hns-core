/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_SESSION_RESTORE_SYNTHESIZED_SESSION_RESTORE_H_
#define HNS_IOS_BROWSER_API_SESSION_RESTORE_SYNTHESIZED_SESSION_RESTORE_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

OBJC_EXPORT
@interface SynthesizedSessionRestore : NSObject
- (instancetype)init NS_UNAVAILABLE;

/// Converts session info to NSData for use with WKWebView::interactionState
/// title - The page title
/// urls - The navigation stack URLs
/// pageIndex - Index of the current page within the navigation stack URLs
/// isPrivateBrowsing -True if the page is off the record
/// - returns - The serialized page information
+ (NSData*)serializeWithTitle:(NSString*)title
                  historyURLs:(NSArray<NSURL*>*)urls
                    pageIndex:(NSUInteger)pageIndex
            isPrivateBrowsing:(bool)isPrivateBrowsing;
@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_SESSION_RESTORE_SYNTHESIZED_SESSION_RESTORE_H_

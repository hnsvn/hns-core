/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_SYNC_HNS_SYNC_INTERNALS_H_
#define HNS_IOS_BROWSER_API_SYNC_HNS_SYNC_INTERNALS_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <Webkit/Webkit.h>

NS_ASSUME_NONNULL_BEGIN

OBJC_EXPORT
@interface HnsSyncInternalsController : UIViewController
- (instancetype)init NS_UNAVAILABLE;
@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_SYNC_HNS_SYNC_INTERNALS_H_

/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_HNS_STATS_HNS_STATS_H_
#define HNS_IOS_BROWSER_API_HNS_STATS_HNS_STATS_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface HnsStats : NSObject
/// Any additional wallet parameters to send with the DAU ping
@property(readonly) NSDictionary<NSString*, NSString*>* walletParams;
/// Should be called when the stats ping has been sent
- (void)notifyStatsPingSent;
- (instancetype)init NS_UNAVAILABLE;
@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_HNS_STATS_HNS_STATS_H_

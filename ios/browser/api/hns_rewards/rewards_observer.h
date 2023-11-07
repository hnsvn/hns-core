/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_HNS_REWARDS_REWARDS_OBSERVER_H_
#define HNS_IOS_BROWSER_API_HNS_REWARDS_REWARDS_OBSERVER_H_

#import <Foundation/Foundation.h>
#import "rewards.mojom.objc.h"

@class HnsRewardsAPI, RewardsNotification;

NS_ASSUME_NONNULL_BEGIN

/// A rewards observer can get notified when certain actions happen
///
/// Creating a RewardsObserver alone will not respond to any events. Set
/// each closure that you wish to watch based on the data being displayed on
/// screen
OBJC_EXPORT
NS_SWIFT_NAME(RewardsObserver)
@interface RewardsObserver : NSObject

@property(nonatomic, readonly, weak) HnsRewardsAPI* rewardsAPI;

- (instancetype)initWithRewardsAPI:(HnsRewardsAPI*)rewardsAPI;

/// Executed when the wallet is first initialized
@property(nonatomic, copy, nullable) void (^walletInitalized)
    (HnsRewardsResult result);

/// A publisher was fetched by its URL for a specific tab identified by tabId
@property(nonatomic, copy, nullable) void (^fetchedPanelPublisher)
    (HnsRewardsPublisherInfo* info, uint64_t tabId);

@property(nonatomic, copy, nullable) void (^publisherListUpdated)();

///
@property(nonatomic, copy, nullable) void (^finishedPromotionsAdded)
    (NSArray<HnsRewardsPromotion*>* promotions);

/// Eligable grants were added to the wallet
@property(nonatomic, copy, nullable) void (^promotionsAdded)
    (NSArray<HnsRewardsPromotion*>* promotions);

/// A grant was claimed
@property(nonatomic, copy, nullable) void (^promotionClaimed)
    (HnsRewardsPromotion* promotion);

@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_HNS_REWARDS_REWARDS_OBSERVER_H_

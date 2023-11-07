/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#import "rewards_observer.h"
#import "hns_rewards_api.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

@interface RewardsObserver ()
@property(nonatomic, weak) HnsRewardsAPI* rewardsAPI;
@end

@implementation RewardsObserver

- (instancetype)initWithRewardsAPI:(HnsRewardsAPI*)rewardsAPI {
  if ((self = [super init])) {
    self.rewardsAPI = rewardsAPI;
  }
  return self;
}

@end

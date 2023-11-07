/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_P3A_HNS_HISTOGRAMS_CONTROLLER_PRIVATE_H_
#define HNS_IOS_BROWSER_API_P3A_HNS_HISTOGRAMS_CONTROLLER_PRIVATE_H_

#include "hns/ios/browser/api/p3a/hns_histograms_controller.h"

class ChromeBrowserState;

NS_ASSUME_NONNULL_BEGIN

@interface HnsHistogramsController (Private)
- (instancetype)initWithBrowserState:(ChromeBrowserState*)mainBrowserState;
@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_P3A_HNS_HISTOGRAMS_CONTROLLER_PRIVATE_H_

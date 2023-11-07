/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_P3A_HNS_P3A_UTILS_PRIVATE_H_
#define HNS_IOS_BROWSER_API_P3A_HNS_P3A_UTILS_PRIVATE_H_

#include "hns/ios/browser/api/p3a/hns_p3a_utils.h"

class ChromeBrowserState;
class PrefService;

namespace p3a {
class P3AService;
}  // namespace p3a

NS_ASSUME_NONNULL_BEGIN

@interface HnsP3AUtils (Private)
- (instancetype)initWithBrowserState:(ChromeBrowserState*)mainBrowserState
                          localState:(PrefService*)localState
                          p3aService:(scoped_refptr<p3a::P3AService>)p3aService;
@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_P3A_HNS_P3A_UTILS_PRIVATE_H_

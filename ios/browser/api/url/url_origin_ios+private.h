/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_URL_URL_ORIGIN_IOS_PRIVATE_H_
#define HNS_IOS_BROWSER_API_URL_URL_ORIGIN_IOS_PRIVATE_H_

#import <Foundation/Foundation.h>

namespace url {

class Origin;

}  // namespace url

NS_ASSUME_NONNULL_BEGIN

@interface URLOriginIOS (Private)

/// Returns a copy of the wrapped `url::Origin`
- (url::Origin)underlyingOrigin;

- (instancetype)initWithOrigin:(url::Origin)origin;

@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_URL_URL_ORIGIN_IOS_PRIVATE_H_

// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_IOS_BROWSER_API_URL_SANITIZER_URL_SANITIZER_SERVICE_PRIVATE_H_
#define HNS_IOS_BROWSER_API_URL_SANITIZER_URL_SANITIZER_SERVICE_PRIVATE_H_

#import <Foundation/Foundation.h>

#include "base/memory/raw_ptr.h"
#include "hns/components/url_sanitizer/browser/url_sanitizer_service.h"
#include "hns/ios/browser/api/url_sanitizer/url_sanitizer_service.h"

NS_ASSUME_NONNULL_BEGIN

@interface URLSanitizerService (Private)

- (instancetype)initWithURLSanitizerService:
    (hns::URLSanitizerService*)urlSanitizer;

@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_URL_SANITIZER_URL_SANITIZER_SERVICE_PRIVATE_H_

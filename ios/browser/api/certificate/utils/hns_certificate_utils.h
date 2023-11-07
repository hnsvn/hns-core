/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_CERTIFICATE_UTILS_HNS_CERTIFICATE_UTILS_H_
#define HNS_IOS_BROWSER_API_CERTIFICATE_UTILS_HNS_CERTIFICATE_UTILS_H_

#import <Foundation/Foundation.h>
#include <string>

namespace certificate {
namespace utils {
NSData* NSStringToData(const std::string& str);
}  // namespace utils
}  // namespace certificate

#endif  //  HNS_IOS_BROWSER_API_CERTIFICATE_UTILS_HNS_CERTIFICATE_UTILS_H_

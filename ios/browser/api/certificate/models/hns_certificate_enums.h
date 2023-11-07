/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_CERTIFICATE_MODELS_HNS_CERTIFICATE_ENUMS_H_
#define HNS_IOS_BROWSER_API_CERTIFICATE_MODELS_HNS_CERTIFICATE_ENUMS_H_

#import <Foundation/Foundation.h>

typedef NS_OPTIONS(NSUInteger, HnsPublicKeyUsage) {
  HnsPublicKeyUsage_INVALID = 1 << 0,
  HnsPublicKeyUsage_ENCRYPT = 1 << 1,
  HnsPublicKeyUsage_DECRYPT = 1 << 2,
  HnsPublicKeyUsage_SIGN = 1 << 3,
  HnsPublicKeyUsage_VERIFY = 1 << 4,
  HnsPublicKeyUsage_WRAP = 1 << 5,
  HnsPublicKeyUsage_DERIVE = 1 << 6,
  HnsPublicKeyUsage_ANY = 1 << 7
};

typedef NS_ENUM(NSUInteger, HnsPublicKeyType) {
  HnsPublicKeyType_UNKNOWN,
  HnsPublicKeyType_RSA,
  HnsPublicKeyType_DSA,
  HnsPublicKeyType_DH,
  HnsPublicKeyType_EC
};

typedef NS_ENUM(NSUInteger, HnsFingerprintType) {
  HnsFingerprintType_SHA1,
  HnsFingerprintType_SHA256
};

#endif  // HNS_IOS_BROWSER_API_CERTIFICATE_MODELS_HNS_CERTIFICATE_ENUMS_H_

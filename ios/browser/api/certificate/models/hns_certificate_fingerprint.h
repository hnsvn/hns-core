/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_CERTIFICATE_MODELS_HNS_CERTIFICATE_FINGERPRINT_H_
#define HNS_IOS_BROWSER_API_CERTIFICATE_MODELS_HNS_CERTIFICATE_FINGERPRINT_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, HnsFingerprintType);

OBJC_EXPORT
@interface HnsCertificateFingerprint : NSObject
@property(nonatomic, readonly) HnsFingerprintType type;
@property(nonatomic, readonly) NSString* fingerprintHexEncoded;
@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_CERTIFICATE_MODELS_HNS_CERTIFICATE_FINGERPRINT_H_

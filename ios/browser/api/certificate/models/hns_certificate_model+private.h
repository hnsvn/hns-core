/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_CERTIFICATE_MODELS_HNS_CERTIFICATE_MODEL_PRIVATE_H_
#define HNS_IOS_BROWSER_API_CERTIFICATE_MODELS_HNS_CERTIFICATE_MODEL_PRIVATE_H_

#import <Foundation/Foundation.h>
#import <Security/Security.h>

#include "hns/ios/browser/api/certificate/models/hns_certificate_enums.h"
#include "hns/ios/browser/api/certificate/models/hns_certificate_fingerprint.h"
#include "hns/ios/browser/api/certificate/models/hns_certificate_public_key_info.h"
#include "hns/ios/browser/api/certificate/models/hns_certificate_rdns_sequence.h"
#include "hns/ios/browser/api/certificate/models/hns_certificate_signature.h"

namespace net {
class ParsedCertificate;

namespace der {
class BitString;
class Input;
}  // namespace der
}  // namespace net

typedef NS_ENUM(NSUInteger, HnsFingerprintType);

NS_ASSUME_NONNULL_BEGIN

@interface HnsCertificateSignature ()
- (instancetype)initWithCertificate:(const net::ParsedCertificate*)certificate;
@end

@interface HnsCertificatePublicKeyInfo ()
- (instancetype)initWithCertificate:(const net::ParsedCertificate*)certificate
                            withKey:(SecKeyRef)key;
@end

@interface HnsCertificateFingerprint ()
- (instancetype)initWithCertificate:(CFDataRef)cert_data
                           withType:(HnsFingerprintType)type;
@end

@interface HnsCertificateRDNSequence ()
- (instancetype)initWithBERName:(const net::der::Input&)berName
                       uniqueId:(const net::der::BitString&)uniqueId;
@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_CERTIFICATE_MODELS_HNS_CERTIFICATE_MODEL_PRIVATE_H_

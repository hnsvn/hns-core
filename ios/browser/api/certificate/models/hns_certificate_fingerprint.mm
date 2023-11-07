/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/ios/browser/api/certificate/models/hns_certificate_fingerprint.h"
#include "base/hash/sha1.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/sys_string_conversions.h"
#include "hns/ios/browser/api/certificate/models/hns_certificate_enums.h"
#include "hns/ios/browser/api/certificate/utils/hns_certificate_utils.h"
#include "hns/ios/browser/api/certificate/utils/hns_certificate_x509_utils.h"
#include "crypto/secure_hash.h"
#include "crypto/sha2.h"

@implementation HnsCertificateFingerprint
- (instancetype)initWithCertificate:(CFDataRef)cert_data
                           withType:(HnsFingerprintType)type {
  if ((self = [super init])) {
    _type = type;

    switch (type) {
      case HnsFingerprintType_SHA1: {
        std::string data = base::SHA1HashString(std::string(
            reinterpret_cast<const char*>(CFDataGetBytePtr(cert_data)),
            CFDataGetLength(cert_data)));
        _fingerprintHexEncoded =
            base::SysUTF8ToNSString(base::HexEncode(data.data(), data.size()));

      } break;

      case HnsFingerprintType_SHA256: {
        std::string data(crypto::kSHA256Length, 0);
        std::unique_ptr<crypto::SecureHash> secure_hash(
            crypto::SecureHash::Create(crypto::SecureHash::SHA256));

        secure_hash->Update(CFDataGetBytePtr(cert_data),
                            CFDataGetLength(cert_data));
        secure_hash->Finish(std::data(data), data.size());

        _fingerprintHexEncoded =
            base::SysUTF8ToNSString(base::HexEncode(data.data(), data.size()));
      } break;
    }
  }
  return self;
}
@end

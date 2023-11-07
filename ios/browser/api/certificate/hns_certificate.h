/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_CERTIFICATE_HNS_CERTIFICATE_H_
#define HNS_IOS_BROWSER_API_CERTIFICATE_HNS_CERTIFICATE_H_

#import <Foundation/Foundation.h>
#import <Security/Security.h>

NS_ASSUME_NONNULL_BEGIN

@class HnsCertificateRDNSequence;
@class HnsCertificateSignature;
@class HnsCertificatePublicKeyInfo;
@class HnsCertificateFingerprint;
// @class HnsCertificateExtensionModel;

OBJC_EXPORT
@interface HnsCertificateModel : NSObject
@property(nonatomic, readonly) bool isRootCertificate;
@property(nonatomic, readonly) bool isCertificateAuthority;
@property(nonatomic, readonly) bool isSelfSigned;
@property(nonatomic, readonly) bool isSelfIssued;
@property(nonatomic, readonly) HnsCertificateRDNSequence* subjectName;
@property(nonatomic, readonly) HnsCertificateRDNSequence* issuerName;
@property(nonatomic, readonly) NSString* serialNumber;
@property(nonatomic, readonly) NSUInteger version;
@property(nonatomic, readonly) HnsCertificateSignature* signature;
@property(nonatomic, readonly) NSDate* notValidBefore;
@property(nonatomic, readonly) NSDate* notValidAfter;
@property(nonatomic, readonly) HnsCertificatePublicKeyInfo* publicKeyInfo;
// @property(nonatomic, readonly)
// NSArray<HnsCertificateExtensionModel*>* extensions;
@property(nonatomic, readonly) HnsCertificateFingerprint* sha1Fingerprint;
@property(nonatomic, readonly) HnsCertificateFingerprint* sha256Fingerprint;

- (nullable instancetype)initWithCertificate:(SecCertificateRef)certificate;
@end

NS_ASSUME_NONNULL_END

#endif  // HNS_IOS_BROWSER_API_CERTIFICATE_HNS_CERTIFICATE_H_

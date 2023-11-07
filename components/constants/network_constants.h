/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_CONSTANTS_NETWORK_CONSTANTS_H_
#define HNS_COMPONENTS_CONSTANTS_NETWORK_CONSTANTS_H_

#include "hns/components/widevine/static_buildflags.h"

extern const char kExtensionUpdaterDomain[];

extern const char kHnsProxyPattern[];
extern const char kHnsSoftwareProxyPattern[];

extern const char kHnsUsageStandardPath[];
extern const char kHnsUsageThresholdPath[];
extern const char kHnsReferralsServer[];
extern const char kHnsReferralsInitPath[];
extern const char kHnsReferralsActivityPath[];
extern const char kHnsSafeBrowsing2Proxy[];
extern const char kHnsSafeBrowsingSslProxy[];
extern const char kHnsRedirectorProxy[];
extern const char kHnsClients4Proxy[];
extern const char kHnsStaticProxy[];

extern const char kAutofillPrefix[];
extern const char kClients4Prefix[];
extern const char kCRXDownloadPrefix[];
extern const char kEmptyDataURI[];
extern const char kEmptyImageDataURI[];
extern const char kJSDataURLPrefix[];
extern const char kGeoLocationsPattern[];
extern const char kSafeBrowsingPrefix[];
extern const char kSafeBrowsingCrxListPrefix[];
extern const char kSafeBrowsingFileCheckPrefix[];
extern const char kCRLSetPrefix1[];
extern const char kCRLSetPrefix2[];
extern const char kCRLSetPrefix3[];
extern const char kCRLSetPrefix4[];
extern const char kChromeCastPrefix[];
extern const char kWidevineGvt1Prefix[];
extern const char kWidevineGoogleDlPrefix[];

#if BUILDFLAG(WIDEVINE_ARM64_DLL_FIX)
extern const char kWidevineGoogleDlPrefixWinArm64[];
#endif

extern const char kUserAgentHeader[];
extern const char kHnsPartnerHeader[];
extern const char kHnsServicesKeyHeader[];

extern const char kBittorrentMimeType[];
extern const char kOctetStreamMimeType[];

extern const char kSecGpcHeader[];
#endif  // HNS_COMPONENTS_CONSTANTS_NETWORK_CONSTANTS_H_

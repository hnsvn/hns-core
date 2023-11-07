/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/constants/network_constants.h"

const char kExtensionUpdaterDomain[] = "extensionupdater.hns.com";

const char kHnsProxyPattern[] = "https://*.hns.com/*";
const char kHnsSoftwareProxyPattern[] = "https://*.hnssoftware.com/*";

const char kHnsUsageStandardPath[] = "/1/usage/hns-core";
const char kHnsUsageThresholdPath[] = "/1/usage/hns-core-threshold";

const char kHnsReferralsServer[] = "laptop-updates.hns.com";
const char kHnsReferralsInitPath[] = "/promo/initialize/nonua";
const char kHnsReferralsActivityPath[] = "/promo/activity";

const char kHnsSafeBrowsing2Proxy[] = "safebrowsing2.hns.com";
const char kHnsSafeBrowsingSslProxy[] = "sb-ssl.hns.com";
const char kHnsRedirectorProxy[] = "redirector.hns.com";
const char kHnsClients4Proxy[] = "clients4.hns.com";
const char kHnsStaticProxy[] = "static1.hns.com";

const char kAutofillPrefix[] = "https://www.gstatic.com/autofill/*";
const char kClients4Prefix[] = "*://clients4.google.com/";
const char kCRXDownloadPrefix[] =
    "*://clients2.googleusercontent.com/crx/blobs/*crx*";
const char kEmptyDataURI[] = "data:text/plain,";
const char kEmptyImageDataURI[] =
    "data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///"
    "yH5BAEAAAAALAAAAAABAAEAAAIBRAA7";
const char kJSDataURLPrefix[] = "data:application/javascript;base64,";
const char kGeoLocationsPattern[] =
    "https://www.googleapis.com/geolocation/v1/geolocate?key=*";
const char kSafeBrowsingPrefix[] = "https://safebrowsing.googleapis.com/";
const char kSafeBrowsingCrxListPrefix[] =
    "https://safebrowsing.google.com/safebrowsing/clientreport/crx-list-info";
const char kSafeBrowsingFileCheckPrefix[] =
    "https://sb-ssl.google.com/safebrowsing/clientreport/download";
const char kCRLSetPrefix1[] =
    "*://dl.google.com/release2/chrome_component/*crl-set*";
const char kCRLSetPrefix2[] =
    "*://*.gvt1.com/edgedl/release2/chrome_component/*";
const char kCRLSetPrefix3[] =
    "*://www.google.com/dl/release2/chrome_component/*";
const char kCRLSetPrefix4[] =
    "*://storage.googleapis.com/update-delta/hfnkpimlhhgieaddgfemjhofmfblmnib"
    "/*crxd";
const char kChromeCastPrefix[] =
    "*://*.gvt1.com/edgedl/chromewebstore/*pkedcjkdefgpdelpbcmbmeomcjbeemfm*";

const char kWidevineGvt1Prefix[] =
    "*://*.gvt1.com/*oimompecagnajdejgnnjijobebaeigek*";
const char kWidevineGoogleDlPrefix[] =
    "*://dl.google.com/*oimompecagnajdejgnnjijobebaeigek*";

#if BUILDFLAG(WIDEVINE_ARM64_DLL_FIX)
const char kWidevineGoogleDlPrefixWinArm64[] =
    "*://dl.google.com/widevine-cdm/*";
#endif

const char kUserAgentHeader[] = "User-Agent";
const char kHnsPartnerHeader[] = "X-Hns-Partner";
const char kHnsServicesKeyHeader[] = "HnsServiceKey";

const char kBittorrentMimeType[] = "application/x-bittorrent";
const char kOctetStreamMimeType[] = "application/octet-stream";

const char kSecGpcHeader[] = "Sec-GPC";

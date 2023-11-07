/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_NET_HNS_NETWORK_AUDIT_ALLOWED_LISTS_H_
#define HNS_BROWSER_NET_HNS_NETWORK_AUDIT_ALLOWED_LISTS_H_

namespace hns {

// Before adding to this list, get approval from the security team.
constexpr const char* kAllowedUrlProtocols[] = {
    "chrome-extension", "chrome", "hns", "file", "data", "blob",
};

// Before adding to this list, get approval from the security team.
constexpr const char* kAllowedUrlPrefixes[] = {
    // allowed because it 307's to https://componentupdater.hns.com
    "https://componentupdater.hns.com/service/update2",
    "https://crxdownload.hns.com/crx/blobs/",

    // Omaha/Sparkle
    "https://updates.hnssoftware.com/",

    // stats/referrals
    "https://laptop-updates.hns.com/",
    "https://laptop-updates-staging.hns.com/",

    // needed for DoH on Mac build machines
    "https://dns.google/dns-query",

    // needed for DoH on Mac build machines
    "https://chrome.cloudflare-dns.com/dns-query",

    // for fetching tor client updater component
    "https://tor.hnssoftware.com/",

    // hns sync v2 production
    "https://sync-v2.hns.com/v2",

    // hns sync v2 staging
    "https://sync-v2.hnssoftware.com/v2",

    // hns sync v2 dev
    "https://sync-v2.hns.software/v2",

    // hns A/B testing
    "https://variations.hns.com/seed",

    // Hns News (production)
    "https://hns-today-cdn.hns.com/",

    // Hns's Privacy-focused CDN
    "https://pcdn.hns.com/",

    // Hns Rewards production
    "https://api.rewards.hns.com/v1/parameters",
    "https://rewards.hns.com/publishers/prefix-list",
    "https://grant.rewards.hns.com/v1/promotions",

    // Hns Rewards staging & dev
    "https://api.rewards.hnssoftware.com/v1/parameters",
    "https://rewards-stg.hnssoftware.com/publishers/prefix-list",
    "https://grant.rewards.hnssoftware.com/v1/promotions",

    // Other
    "https://hns-core-ext.s3.hns.com/",
    "https://dict.hns.com/",
    "https://go-updater.hns.com/",
    "https://p3a.hns.com/",
    "https://p3a-creative.hns.com/",
    "https://p3a-json.hns.com/",
    "https://redirector.hns.com/",
    "https://safebrowsing.hns.com/",
    "https://static.hns.com/",
    "https://static1.hns.com/",
};

// Before adding to this list, get approval from the security team.
constexpr const char* kAllowedUrlPatterns[] = {
    // allowed because it's url for fetching super referral's mapping table
    "https://mobile-data.s3.hns.com/superreferrer/map-table.json",
    "https://mobile-data-dev.s3.hns.software/superreferrer/map-table.json",
};

}  // namespace hns

#endif  // HNS_BROWSER_NET_HNS_NETWORK_AUDIT_ALLOWED_LISTS_H_

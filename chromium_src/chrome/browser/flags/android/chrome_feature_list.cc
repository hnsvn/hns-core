/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/feature_override.h"
#include "hns/browser/android/preferences/features.h"
#include "hns/browser/android/safe_browsing/features.h"
#include "hns/components/hns_news/common/features.h"
#include "hns/components/hns_rewards/common/features.h"
#include "hns/components/hns_search_conversion/features.h"
#include "hns/components/hns_shields/common/features.h"
#include "hns/components/hns_vpn/common/features.h"
#include "hns/components/hns_wallet/common/features.h"
#include "hns/components/debounce/common/features.h"
#include "hns/components/google_sign_in_permission/features.h"
#include "hns/components/playlist/common/features.h"
#include "hns/components/request_otr/common/features.h"
#include "hns/components/speedreader/common/features.h"
#include "net/base/features.h"
#include "third_party/blink/public/common/features.h"

// clang-format off
#define kForceWebContentsDarkMode kForceWebContentsDarkMode,            \
    &hns_rewards::features::kHnsRewards,                            \
    &hns_search_conversion::features::kOmniboxBanner,                 \
    &hns_vpn::features::kHnsVPNLinkSubscriptionAndroidUI,           \
    &hns_wallet::features::kNativeHnsWalletFeature,                 \
    &hns_wallet::features::kHnsWalletSolanaFeature,                 \
    &hns_wallet::features::kHnsWalletFilecoinFeature,               \
    &hns_wallet::features::kHnsWalletSnsFeature,                    \
    &playlist::features::kPlaylist,                                     \
    &preferences::features::kHnsBackgroundVideoPlayback,              \
    &request_otr::features::kHnsRequestOTRTab,                        \
    &safe_browsing::features::kHnsAndroidSafeBrowsing,                \
    &speedreader::kSpeedreaderFeature,                                  \
    &debounce::features::kHnsDebounce,                                \
    &net::features::kHnsHttpsByDefault,                               \
    &google_sign_in_permission::features::kHnsGoogleSignInPermission, \
    &net::features::kHnsForgetFirstPartyStorage,                      \
    &hns_shields::features::kHnsLocalhostAccessPermission

// clang-format on

#include "src/chrome/browser/flags/android/chrome_feature_list.cc"
#undef kForceWebContentsDarkMode

namespace chrome {
namespace android {

OVERRIDE_FEATURE_DEFAULT_STATES({{
    {kAddToHomescreenIPH, base::FEATURE_DISABLED_BY_DEFAULT},
    {kBaselineGM3SurfaceColors, base::FEATURE_DISABLED_BY_DEFAULT},
    // Disable until we sort out UI issues
    // https://github.com/hnsvn/hns-browser/issues/29688
    {kIncognitoReauthenticationForAndroid, base::FEATURE_DISABLED_BY_DEFAULT},
    {kShowScrollableMVTOnNTPAndroid, base::FEATURE_ENABLED_BY_DEFAULT},
    {kStartSurfaceAndroid, base::FEATURE_DISABLED_BY_DEFAULT},
}});

}  // namespace android
}  // namespace chrome

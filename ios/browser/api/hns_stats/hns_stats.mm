/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/ios/browser/api/hns_stats/hns_stats.h"

#include "base/strings/sys_string_conversions.h"
#include "hns/components/hns_stats/browser/hns_stats_updater_util.h"
#include "hns/components/hns_wallet/browser/pref_names.h"
#include "components/prefs/pref_service.h"
#include "ios/chrome/browser/shared/model/application_context/application_context.h"
#include "ios/chrome/browser/shared/model/browser_state/chrome_browser_state.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

@implementation HnsStats {
  PrefService* _localPrefs;
  PrefService* _profilePrefs;
}

- (instancetype)initWithBrowserState:(ChromeBrowserState*)browserState {
  if ((self = [super init])) {
    _profilePrefs = browserState->GetPrefs();
    _localPrefs = GetApplicationContext()->GetLocalState();
  }
  return self;
}

- (NSDictionary<NSString*, NSString*>*)walletParams {
  auto wallet_last_unlocked = _localPrefs->GetTime(kHnsWalletLastUnlockTime);
  auto last_reported_wallet_unlock =
      _localPrefs->GetTime(kHnsWalletPingReportedUnlockTime);
  uint8_t usage_bitset = 0;
  if (wallet_last_unlocked > last_reported_wallet_unlock) {
    usage_bitset = hns_stats::UsageBitfieldFromTimestamp(
        wallet_last_unlocked, last_reported_wallet_unlock);
  }
  return @{@"wallet2" : base::SysUTF8ToNSString(std::to_string(usage_bitset))};
}

- (void)notifyStatsPingSent {
  auto wallet_last_unlocked = _localPrefs->GetTime(kHnsWalletLastUnlockTime);
  _localPrefs->SetTime(kHnsWalletPingReportedUnlockTime,
                       wallet_last_unlocked);
}

@end

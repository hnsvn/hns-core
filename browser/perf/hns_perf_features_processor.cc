// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "hns/browser/perf/hns_perf_features_processor.h"

#include "base/command_line.h"
#include "base/task/sequenced_task_runner.h"
#include "hns/browser/hns_browser_process.h"
#include "hns/browser/hns_rewards/rewards_service_factory.h"
#include "hns/browser/perf/hns_perf_switches.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"
#include "hns/components/hns_news/common/pref_names.h"
#include "hns/components/hns_rewards/browser/rewards_service.h"
#include "hns/components/hns_shields/browser/ad_block_regional_service_manager.h"
#include "hns/components/hns_shields/browser/ad_block_service.h"
#include "hns/components/hns_shields/common/hns_shield_constants.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/chrome_switches.h"
#include "components/prefs/pref_service.h"

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "hns/components/speedreader/speedreader_pref_names.h"
#endif

namespace {
void FakeCallback(hns_rewards::mojom::CreateRewardsWalletResult) {}

void EnableAdblockCookieList(base::WeakPtr<Profile> profile) {
  if (!profile) {
    return;
  }

  // Obtrusive cookie notices list in cosmetic filters.
  auto* regional_service_manager =
      g_hns_browser_process->ad_block_service()->regional_service_manager();
  if (!regional_service_manager ||
      !regional_service_manager->IsFilterListAvailable(
          hns_shields::kCookieListUuid)) {
    base::SequencedTaskRunner::GetCurrentDefault()->PostDelayedTask(
        FROM_HERE, base::BindOnce(&EnableAdblockCookieList, profile),
        base::Seconds(1));
    return;
  }

  regional_service_manager->EnableFilterList(hns_shields::kCookieListUuid,
                                             true);
}

}  // namespace

namespace perf {

void MaybeEnableHnsFeatureForPerfTesting(Profile* profile) {
  auto* cmd = base::CommandLine::ForCurrentProcess();
  if (!cmd->HasSwitch(switches::kEnableHnsFeaturesForPerfTesting) ||
      !cmd->HasSwitch(::switches::kUserDataDir)) {
    return;
  }

  // Notification Ads
  profile->GetPrefs()->SetBoolean(hns_ads::prefs::kOptedInToNotificationAds,
                                  true);

  // Rewards
  auto* rewards_service =
      hns_rewards::RewardsServiceFactory::GetForProfile(profile);
  rewards_service->CreateRewardsWallet("US", base::BindOnce(&FakeCallback));

  // Hns news
  profile->GetPrefs()->SetBoolean(hns_news::prefs::kNewTabPageShowToday,
                                  true);
  profile->GetPrefs()->SetBoolean(hns_news::prefs::kHnsNewsOptedIn, true);

#if BUILDFLAG(ENABLE_SPEEDREADER)
  // Speedreader
  profile->GetPrefs()->SetBoolean(speedreader::kSpeedreaderPrefEnabled, true);
#endif

  // Adblock
  EnableAdblockCookieList(profile->GetWeakPtr());
}

}  // namespace perf

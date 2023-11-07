// Copyright (c) 2019 The Hns Authors
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "hns/browser/ui/webui/navigation_bar_data_provider.h"

#include "hns/browser/hns_rewards/rewards_util.h"
#include "hns/browser/hns_wallet/hns_wallet_context_utils.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/grit/generated_resources.h"
#include "components/strings/grit/components_strings.h"
#include "content/public/browser/web_ui_data_source.h"

#include "components/grit/hns_components_strings.h"

// static
void NavigationBarDataProvider::Initialize(content::WebUIDataSource* source,
                                           Profile* profile) {
  source->AddLocalizedString("brToolbarSettingsTitle", IDS_SETTINGS_SETTINGS);
  source->AddLocalizedString("brToolbarBookmarksTitle",
                             IDS_BOOKMARK_MANAGER_TITLE);
  source->AddLocalizedString("brToolbarDownloadsTitle", IDS_DOWNLOAD_TITLE);
  source->AddLocalizedString("brToolbarHistoryTitle", IDS_HISTORY_TITLE);
  source->AddLocalizedString("brToolbarRewardsTitle",
                             IDS_HNS_UI_HNS_REWARDS);
  source->AddLocalizedString("brToolbarWalletsTitle", IDS_WALLETS_TITLE);

  source->AddBoolean(
      "brToolbarShowRewardsButton",
      hns_rewards::IsSupportedForProfile(
          profile, hns_rewards::IsSupportedOptions::kSkipRegionCheck));
  source->AddBoolean("isHnsWalletAllowed",
                     hns_wallet::IsAllowedForContext(profile));
}

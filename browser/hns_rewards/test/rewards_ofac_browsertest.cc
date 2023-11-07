/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/app/hns_command_ids.h"
#include "hns/browser/hns_ads/ads_service_factory.h"
#include "hns/browser/hns_rewards/rewards_service_factory.h"
#include "hns/browser/hns_rewards/rewards_util.h"
#include "hns/components/hns_rewards/browser/rewards_service.h"
#include "hns/components/hns_rewards/common/rewards_util.h"
#include "hns/components/l10n/common/test/scoped_default_locale.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_command_controller.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "components/prefs/pref_service.h"
#include "components/user_prefs/user_prefs.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/web_contents.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_utils.h"
#include "url/gurl.h"

namespace hns_rewards {

class HnsRewardsOFACTest : public InProcessBrowserTest {
 public:
  HnsRewardsOFACTest() = default;
  ~HnsRewardsOFACTest() override = default;

  content::WebContents* web_contents() const {
    return browser()->tab_strip_model()->GetActiveWebContents();
  }

  content::BrowserContext* browser_context() {
    return web_contents()->GetBrowserContext();
  }

  Profile* profile() { return browser()->profile(); }

  PrefService* prefs() { return user_prefs::UserPrefs::Get(browser_context()); }
};

// Verify that hns_rewards::IsSupported works correctly based on the locale.
IN_PROC_BROWSER_TEST_F(HnsRewardsOFACTest, IsHnsRewardsDisabled) {
  {
    const hns_l10n::test::ScopedDefaultLocale locale("en_CA");  // "Canada"
    EXPECT_FALSE(IsUnsupportedRegion());
    EXPECT_TRUE(hns_rewards::IsSupported(prefs()));
    EXPECT_TRUE(hns_rewards::IsSupported(
        prefs(), hns_rewards::IsSupportedOptions::kSkipRegionCheck));
    EXPECT_TRUE(hns_rewards::IsSupportedForProfile(profile()));
    EXPECT_TRUE(hns_rewards::IsSupportedForProfile(
        profile(), hns_rewards::IsSupportedOptions::kSkipRegionCheck));
  }
  {
    const hns_l10n::test::ScopedDefaultLocale locale("es_CU");  // "Cuba"
    EXPECT_TRUE(IsUnsupportedRegion());
    EXPECT_FALSE(hns_rewards::IsSupported(prefs()));
    EXPECT_TRUE(hns_rewards::IsSupported(
        prefs(), hns_rewards::IsSupportedOptions::kSkipRegionCheck));
    EXPECT_FALSE(hns_rewards::IsSupportedForProfile(profile()));
    EXPECT_TRUE(hns_rewards::IsSupportedForProfile(
        profile(), hns_rewards::IsSupportedOptions::kSkipRegionCheck));
  }
}

// Verify that Rewards and Ads services don't get created when in an OFAC
// sanctioned region.
IN_PROC_BROWSER_TEST_F(HnsRewardsOFACTest, GetRewardsAndAdsServices) {
  {
    const hns_l10n::test::ScopedDefaultLocale locale("en_CA");  // "Canada"
    EXPECT_NE(hns_rewards::RewardsServiceFactory::GetForProfile(profile()),
              nullptr);
    EXPECT_NE(hns_ads::AdsServiceFactory::GetForProfile(profile()), nullptr);
  }

  {
    const hns_l10n::test::ScopedDefaultLocale locale("es_CU");  // "Cuba"
    EXPECT_EQ(hns_rewards::RewardsServiceFactory::GetForProfile(profile()),
              nullptr);
    EXPECT_EQ(hns_ads::AdsServiceFactory::GetForProfile(profile()), nullptr);
  }
}

// Verify that Rewards menu item is enabled in the app menu even when in an OFAC
// sanctioned region.
IN_PROC_BROWSER_TEST_F(HnsRewardsOFACTest, AppMenuItemEnabled) {
  auto* command_controller = browser()->command_controller();
  {
    const hns_l10n::test::ScopedDefaultLocale locale("en_CA");  // "Canada"
    EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_HNS_REWARDS));
  }
  {
    const hns_l10n::test::ScopedDefaultLocale locale("es_CU");  // "Cuba"
    EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_HNS_REWARDS));
  }
}

// Verify that hns://rewards page is reachable even when  in an OFAC
// sanctioned region.
IN_PROC_BROWSER_TEST_F(HnsRewardsOFACTest, RewardsPagesAccess) {
  const GURL url("chrome://rewards");

  {
    const hns_l10n::test::ScopedDefaultLocale locale("en_CA");  // "Canada"
    auto* rfh = ui_test_utils::NavigateToURL(browser(), url);
    EXPECT_TRUE(rfh);
    EXPECT_FALSE(rfh->IsErrorDocument());
  }

  {
    const hns_l10n::test::ScopedDefaultLocale locale("es_CU");  // "Cuba"
    auto* rfh = ui_test_utils::NavigateToURL(browser(), url);
    EXPECT_TRUE(rfh);
    EXPECT_FALSE(rfh->IsErrorDocument());
  }
}

}  // namespace hns_rewards

/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/strings/stringprintf.h"
#include "hns/app/hns_command_ids.h"
#include "hns/browser/hns_ads/ads_service_factory.h"
#include "hns/browser/hns_rewards/rewards_service_factory.h"
#include "hns/browser/hns_rewards/rewards_util.h"
#include "hns/browser/ui/views/hns_actions/hns_actions_container.h"
#include "hns/browser/ui/views/hns_actions/hns_rewards_action_view.h"
#include "hns/browser/ui/views/location_bar/hns_location_bar_view.h"
#include "hns/components/hns_rewards/browser/rewards_service.h"
#include "hns/components/hns_rewards/common/pref_names.h"
#include "hns/components/hns_rewards/common/rewards_util.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_command_controller.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "components/policy/core/browser/browser_policy_connector.h"
#include "components/policy/core/common/mock_configuration_policy_provider.h"
#include "components/policy/core/common/policy_map.h"
#include "components/policy/policy_constants.h"
#include "components/prefs/pref_service.h"
#include "components/user_prefs/user_prefs.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/web_contents.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_utils.h"
#include "content/public/test/navigation_handle_observer.h"
#include "url/gurl.h"

namespace policy {

class HnsRewardsPolicyTest : public InProcessBrowserTest,
                               public ::testing::WithParamInterface<bool> {
 public:
  HnsRewardsPolicyTest() = default;
  ~HnsRewardsPolicyTest() override = default;

  void SetUpInProcessBrowserTestFixture() override {
    EXPECT_CALL(provider_, IsInitializationComplete(testing::_))
        .WillRepeatedly(testing::Return(true));
    BrowserPolicyConnector::SetPolicyProviderForTesting(&provider_);
    PolicyMap policies;
    policies.Set(key::kHnsRewardsDisabled, POLICY_LEVEL_MANDATORY,
                 POLICY_SCOPE_USER, POLICY_SOURCE_PLATFORM,
                 base::Value(IsHnsRewardsDisabledTest()), nullptr);
    provider_.UpdateChromePolicy(policies);
  }

  bool IsHnsRewardsDisabledTest() { return GetParam(); }

  content::WebContents* web_contents() const {
    return browser()->tab_strip_model()->GetActiveWebContents();
  }

  content::BrowserContext* browser_context() {
    return web_contents()->GetBrowserContext();
  }

  Profile* profile() { return browser()->profile(); }

  PrefService* prefs() { return user_prefs::UserPrefs::Get(browser_context()); }

 private:
  MockConfigurationPolicyProvider provider_;
};

// Verify that hns_rewards::IsDisabledByPolicy works correctly based on the
// preference set by the policy.
IN_PROC_BROWSER_TEST_P(HnsRewardsPolicyTest, IsHnsRewardsDisabled) {
  EXPECT_TRUE(prefs()->FindPreference(hns_rewards::prefs::kDisabledByPolicy));
  if (IsHnsRewardsDisabledTest()) {
    EXPECT_TRUE(prefs()->GetBoolean(hns_rewards::prefs::kDisabledByPolicy));
    EXPECT_FALSE(hns_rewards::IsSupported(prefs()));
    EXPECT_FALSE(hns_rewards::IsSupportedForProfile(profile()));
  } else {
    EXPECT_FALSE(prefs()->GetBoolean(hns_rewards::prefs::kDisabledByPolicy));
    EXPECT_TRUE(hns_rewards::IsSupported(prefs()));
    EXPECT_TRUE(hns_rewards::IsSupportedForProfile(profile()));
  }
}

// Verify that Rewards and Ads services don't get created when Hns Rewards are
// disabled by policy.
IN_PROC_BROWSER_TEST_P(HnsRewardsPolicyTest, GetRewardsAndAdsServices) {
  if (IsHnsRewardsDisabledTest()) {
    EXPECT_EQ(hns_rewards::RewardsServiceFactory::GetForProfile(profile()),
              nullptr);
    EXPECT_EQ(hns_ads::AdsServiceFactory::GetForProfile(profile()), nullptr);
  } else {
    EXPECT_NE(hns_rewards::RewardsServiceFactory::GetForProfile(profile()),
              nullptr);
    EXPECT_NE(hns_ads::AdsServiceFactory::GetForProfile(profile()), nullptr);
  }
}

// Verify that Rewards menu item isn't enabled in the app menu when Hns
// Rewards are disabled by policy.
IN_PROC_BROWSER_TEST_P(HnsRewardsPolicyTest, AppMenuItemDisabled) {
  auto* command_controller = browser()->command_controller();
  if (IsHnsRewardsDisabledTest()) {
    EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_SHOW_HNS_REWARDS));
  } else {
    EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_HNS_REWARDS));
  }
}

// Verify that hns://rewards and hns://rewards-internals pages aren't
// reachable when Hns Rewards are disabled by policy.
IN_PROC_BROWSER_TEST_P(HnsRewardsPolicyTest, RewardsPagesAccess) {
  for (const auto& url :
       {GURL("chrome://rewards"), GURL("chrome://rewards-internals")}) {
    SCOPED_TRACE(testing::Message() << "url=" << url);
    auto* rfh = ui_test_utils::NavigateToURL(browser(), url);
    EXPECT_TRUE(rfh);
    EXPECT_EQ(IsHnsRewardsDisabledTest(), rfh->IsErrorDocument());
  }
}

// Verify that Hns Rewards icon is not shown in the location bar when Hns
// Rewards are disabled by policy.
IN_PROC_BROWSER_TEST_P(HnsRewardsPolicyTest, RewardsIconIsHidden) {
  const auto* browser_view = BrowserView::GetBrowserViewForBrowser(browser());
  ASSERT_NE(browser_view, nullptr);
  const auto* hns_location_bar_view =
      static_cast<HnsLocationBarView*>(browser_view->GetLocationBarView());
  ASSERT_NE(hns_location_bar_view, nullptr);
  const auto* hns_actions = hns_location_bar_view->hns_actions_.get();
  ASSERT_NE(hns_actions, nullptr);
  EXPECT_TRUE(
      prefs()->GetBoolean(hns_rewards::prefs::kShowLocationBarButton));
  if (IsHnsRewardsDisabledTest()) {
    EXPECT_FALSE(hns_actions->rewards_action_btn_->GetVisible());
  } else {
    EXPECT_TRUE(hns_actions->rewards_action_btn_->GetVisible());
  }
}

INSTANTIATE_TEST_SUITE_P(
    HnsRewardsPolicyTest,
    HnsRewardsPolicyTest,
    ::testing::Bool(),
    [](const testing::TestParamInfo<HnsRewardsPolicyTest::ParamType>& info) {
      return base::StringPrintf("HnsRewards_%sByPolicy",
                                info.param ? "Disabled" : "NotDisabled");
    });

}  // namespace policy

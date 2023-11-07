/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "base/strings/stringprintf.h"
#include "hns/app/hns_command_ids.h"
#include "hns/browser/hns_wallet/hns_wallet_context_utils.h"
#include "hns/browser/hns_wallet/hns_wallet_service_factory.h"
#include "hns/browser/ui/hns_browser.h"
#include "hns/browser/ui/sidebar/sidebar_controller.h"
#include "hns/browser/ui/sidebar/sidebar_model.h"
#include "hns/components/hns_wallet/common/common_utils.h"
#include "hns/components/hns_wallet/common/pref_names.h"
#include "hns/components/sidebar/sidebar_item.h"
#include "chrome/browser/profiles/profile.h"
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
#include "extensions/common/constants.h"
#include "url/gurl.h"

namespace policy {

class HnsWalletPolicyTest : public InProcessBrowserTest,
                              public ::testing::WithParamInterface<bool> {
 public:
  HnsWalletPolicyTest() = default;
  ~HnsWalletPolicyTest() override = default;

  void SetUpInProcessBrowserTestFixture() override {
    EXPECT_CALL(provider_, IsInitializationComplete(testing::_))
        .WillRepeatedly(testing::Return(true));
    BrowserPolicyConnector::SetPolicyProviderForTesting(&provider_);
    PolicyMap policies;
    policies.Set(key::kHnsWalletDisabled, POLICY_LEVEL_MANDATORY,
                 POLICY_SCOPE_USER, POLICY_SOURCE_PLATFORM,
                 base::Value(IsHnsWalletDisabledTest()), nullptr);
    provider_.UpdateChromePolicy(policies);
  }

  bool IsHnsWalletDisabledTest() { return GetParam(); }

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

// Verify that hns_wallet::IsDisabledByPolicy works correctly based on the
// preference set by the policy.
IN_PROC_BROWSER_TEST_P(HnsWalletPolicyTest, IsHnsWalletDisabled) {
  EXPECT_TRUE(prefs()->FindPreference(hns_wallet::prefs::kDisabledByPolicy));
  if (IsHnsWalletDisabledTest()) {
    EXPECT_TRUE(prefs()->GetBoolean(hns_wallet::prefs::kDisabledByPolicy));
    EXPECT_FALSE(hns_wallet::IsAllowed(prefs()));
    EXPECT_FALSE(hns_wallet::IsAllowedForContext(profile()));
  } else {
    EXPECT_FALSE(prefs()->GetBoolean(hns_wallet::prefs::kDisabledByPolicy));
    EXPECT_TRUE(hns_wallet::IsAllowed(prefs()));
    EXPECT_TRUE(hns_wallet::IsAllowedForContext(profile()));
  }
}

// Verify that Wallet service doesn't get created when Hns Wallet is
// disabled by policy.
IN_PROC_BROWSER_TEST_P(HnsWalletPolicyTest, GetWalletService) {
  if (IsHnsWalletDisabledTest()) {
    EXPECT_EQ(hns_wallet::HnsWalletServiceFactory::GetServiceForContext(
                  profile()),
              nullptr);
  } else {
    EXPECT_NE(hns_wallet::HnsWalletServiceFactory::GetServiceForContext(
                  profile()),
              nullptr);
  }
}

// Verify that Wallet menu item isn't enabled in the app menu when Hns
// Wallet is disabled by policy.
IN_PROC_BROWSER_TEST_P(HnsWalletPolicyTest, AppMenuItemDisabled) {
  auto* command_controller = browser()->command_controller();
  if (IsHnsWalletDisabledTest()) {
    EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_SHOW_HNS_WALLET));
  } else {
    EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_HNS_WALLET));
  }
}

// Verify that hns://wallet page isn't reachable when Hns Wallet is
// disabled by policy.
IN_PROC_BROWSER_TEST_P(HnsWalletPolicyTest, WalletPageAccess) {
  const GURL url("chrome://wallet");
  auto* rfh = ui_test_utils::NavigateToURL(browser(), url);
  EXPECT_TRUE(rfh);
  EXPECT_EQ(IsHnsWalletDisabledTest(), rfh->IsErrorDocument());
}

// Verify that the wallet item is not shown in the sidebar when Hns Wallet is
// disabled by policy.
IN_PROC_BROWSER_TEST_P(HnsWalletPolicyTest, WalletInSidebar) {
  HnsBrowser* hns_browser = static_cast<HnsBrowser*>(browser());
  sidebar::SidebarController* controller = hns_browser->sidebar_controller();
  sidebar::SidebarModel* model = controller->model();

  const auto items = model->GetAllSidebarItems();
  EXPECT_LT(0UL, items.size());

  const auto iter = base::ranges::find_if(items, [](const auto& i) {
    return (i.built_in_item_type ==
            sidebar::SidebarItem::BuiltInItemType::kWallet);
  });

  if (IsHnsWalletDisabledTest()) {
    EXPECT_TRUE(iter == items.end());
  } else {
    EXPECT_FALSE(iter == items.end());
  }
}

INSTANTIATE_TEST_SUITE_P(
    HnsWalletPolicyTest,
    HnsWalletPolicyTest,
    ::testing::Bool(),
    [](const testing::TestParamInfo<HnsWalletPolicyTest::ParamType>& info) {
      return base::StringPrintf("HnsWallet_%sByPolicy",
                                info.param ? "Disabled" : "NotDisabled");
    });

}  // namespace policy

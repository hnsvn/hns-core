/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>

#include "base/functional/callback_helpers.h"
#include "base/test/scoped_feature_list.h"
#include "hns/browser/ui/hns_browser_command_controller.h"
#include "hns/browser/ui/browser_commands.h"
#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "hns/components/skus/common/features.h"
#include "hns/components/tor/buildflags/buildflags.h"
#include "chrome/app/chrome_command_ids.h"
#include "chrome/browser/chrome_notification_types.h"
#include "chrome/browser/policy/configuration_policy_handler_list_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_window.h"
#include "chrome/browser/ui/browser_list.h"
#include "chrome/browser/ui/browser_list_observer.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "components/policy/core/browser/browser_policy_connector.h"
#include "components/policy/core/common/mock_configuration_policy_provider.h"
#include "components/policy/core/common/policy_map.h"
#include "components/policy/policy_constants.h"
#include "components/prefs/pref_service.h"
#include "components/sync/base/command_line_switches.h"
#include "content/public/browser/notification_service.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/test_utils.h"

#if BUILDFLAG(ENABLE_TOR)
#include "hns/browser/tor/tor_profile_service_factory.h"
#endif

#if BUILDFLAG(ENABLE_HNS_VPN)
#include "hns/browser/hns_vpn/hns_vpn_service_factory.h"
#include "hns/components/hns_vpn/browser/hns_vpn_service.h"
#include "hns/components/hns_vpn/common/hns_vpn_utils.h"
#include "hns/components/hns_vpn/common/features.h"
#include "hns/components/hns_vpn/common/pref_names.h"
#endif

class HnsBrowserCommandControllerTest : public InProcessBrowserTest {
 public:
  HnsBrowserCommandControllerTest() {
#if BUILDFLAG(ENABLE_HNS_VPN)
    scoped_feature_list_.InitWithFeatures(
        {skus::features::kSkusFeature, hns_vpn::features::kHnsVPN}, {});
#endif
  }
#if BUILDFLAG(ENABLE_HNS_VPN)
  void SetUpInProcessBrowserTestFixture() override {
    InProcessBrowserTest::SetUpInProcessBrowserTestFixture();
    provider_.SetDefaultReturns(
        /*is_initialization_complete_return=*/true,
        /*is_first_policy_load_complete_return=*/true);
    policy::BrowserPolicyConnector::SetPolicyProviderForTesting(&provider_);
  }

  void BlockVPNByPolicy(bool value) {
    policy::PolicyMap policies;
    policies.Set(policy::key::kHnsVPNDisabled, policy::POLICY_LEVEL_MANDATORY,
                 policy::POLICY_SCOPE_MACHINE, policy::POLICY_SOURCE_PLATFORM,
                 base::Value(value), nullptr);
    provider_.UpdateChromePolicy(policies);
    EXPECT_EQ(
        hns_vpn::IsHnsVPNDisabledByPolicy(browser()->profile()->GetPrefs()),
        value);
  }

  void SetPurchasedUserForHnsVPN(Browser* browser, bool purchased) {
    auto* service =
        hns_vpn::HnsVpnServiceFactory::GetForProfile(browser->profile());
    ASSERT_TRUE(!!service);
    auto target_state = purchased
                            ? hns_vpn::mojom::PurchasedState::PURCHASED
                            : hns_vpn::mojom::PurchasedState::NOT_PURCHASED;
    service->SetPurchasedState(skus::GetDefaultEnvironment(), target_state);
    // Call explicitely to update vpn commands status because mojo works in
    // async way.
    static_cast<chrome::HnsBrowserCommandController*>(
        browser->command_controller())
        ->OnPurchasedStateChanged(target_state, absl::nullopt);
  }

  void CheckHnsVPNCommands(Browser* browser) {
    // Only IDC_HNS_VPN_MENU command is changed based on purchased state.
    auto* command_controller = browser->command_controller();
    SetPurchasedUserForHnsVPN(browser, false);
    EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_HNS_VPN_PANEL));
    EXPECT_TRUE(command_controller->IsCommandEnabled(
        IDC_TOGGLE_HNS_VPN_TOOLBAR_BUTTON));
    EXPECT_TRUE(
        command_controller->IsCommandEnabled(IDC_SEND_HNS_VPN_FEEDBACK));
    EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_ABOUT_HNS_VPN));
    EXPECT_TRUE(
        command_controller->IsCommandEnabled(IDC_MANAGE_HNS_VPN_PLAN));

    EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_HNS_VPN_MENU));
    EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_TOGGLE_HNS_VPN));

    SetPurchasedUserForHnsVPN(browser, true);
    EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_HNS_VPN_PANEL));
    EXPECT_TRUE(command_controller->IsCommandEnabled(
        IDC_TOGGLE_HNS_VPN_TOOLBAR_BUTTON));
    EXPECT_TRUE(
        command_controller->IsCommandEnabled(IDC_SEND_HNS_VPN_FEEDBACK));
    EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_ABOUT_HNS_VPN));
    EXPECT_TRUE(
        command_controller->IsCommandEnabled(IDC_MANAGE_HNS_VPN_PLAN));

    EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_HNS_VPN_MENU));
    EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_TOGGLE_HNS_VPN));
  }

  void CheckHnsVPNCommandsDisabledByPolicy(Browser* browser) {
    auto* command_controller = browser->command_controller();
    SetPurchasedUserForHnsVPN(browser, false);
    EXPECT_FALSE(
        command_controller->IsCommandEnabled(IDC_SHOW_HNS_VPN_PANEL));
    EXPECT_FALSE(command_controller->IsCommandEnabled(
        IDC_TOGGLE_HNS_VPN_TOOLBAR_BUTTON));
    EXPECT_FALSE(
        command_controller->IsCommandEnabled(IDC_SEND_HNS_VPN_FEEDBACK));
    EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_ABOUT_HNS_VPN));
    EXPECT_FALSE(
        command_controller->IsCommandEnabled(IDC_MANAGE_HNS_VPN_PLAN));

    EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_HNS_VPN_MENU));
    EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_TOGGLE_HNS_VPN));

    SetPurchasedUserForHnsVPN(browser, true);
    EXPECT_FALSE(
        command_controller->IsCommandEnabled(IDC_SHOW_HNS_VPN_PANEL));
    EXPECT_FALSE(command_controller->IsCommandEnabled(
        IDC_TOGGLE_HNS_VPN_TOOLBAR_BUTTON));
    EXPECT_FALSE(
        command_controller->IsCommandEnabled(IDC_SEND_HNS_VPN_FEEDBACK));
    EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_ABOUT_HNS_VPN));
    EXPECT_FALSE(
        command_controller->IsCommandEnabled(IDC_MANAGE_HNS_VPN_PLAN));

    EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_HNS_VPN_MENU));
    EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_TOGGLE_HNS_VPN));
  }

 private:
  policy::MockConfigurationPolicyProvider provider_;
  base::test::ScopedFeatureList scoped_feature_list_;
#endif
};

// Regular window
IN_PROC_BROWSER_TEST_F(HnsBrowserCommandControllerTest,
                       HnsCommandsEnableTest) {
  // Test normal browser's hns commands status.
  auto* command_controller = browser()->command_controller();
  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_HNS_REWARDS));

#if BUILDFLAG(ENABLE_TOR)
  EXPECT_FALSE(
      command_controller->IsCommandEnabled(IDC_NEW_TOR_CONNECTION_FOR_SITE));
  EXPECT_TRUE(
      command_controller->IsCommandEnabled(IDC_NEW_OFFTHERECORD_WINDOW_TOR));
#else
  EXPECT_FALSE(
      command_controller->IsCommandEnabled(IDC_NEW_TOR_CONNECTION_FOR_SITE));
  EXPECT_FALSE(
      command_controller->IsCommandEnabled(IDC_NEW_OFFTHERECORD_WINDOW_TOR));
#endif

#if BUILDFLAG(ENABLE_HNS_VPN)
  EXPECT_FALSE(
      hns_vpn::IsHnsVPNDisabledByPolicy(browser()->profile()->GetPrefs()));
  CheckHnsVPNCommands(browser());
  BlockVPNByPolicy(true);
  CheckHnsVPNCommandsDisabledByPolicy(browser());
  BlockVPNByPolicy(false);
  CheckHnsVPNCommands(browser());
#endif

  if (syncer::IsSyncAllowedByFlag())
    EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_HNS_SYNC));
  else
    EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_SHOW_HNS_SYNC));

  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_HNS_WALLET));

  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_ADD_NEW_PROFILE));
  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_OPEN_GUEST_PROFILE));
  EXPECT_TRUE(
      command_controller->IsCommandEnabled(IDC_SHOW_HNS_WEBCOMPAT_REPORTER));

  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_TOGGLE_SIDEBAR));
}

// Create private browser and test its hns commands status.
IN_PROC_BROWSER_TEST_F(HnsBrowserCommandControllerTest,
                       HnsCommandsEnableTestPrivateWindow) {
  auto* private_browser = CreateIncognitoBrowser();
  auto* command_controller = private_browser->command_controller();
  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_HNS_REWARDS));

#if BUILDFLAG(ENABLE_TOR)
  EXPECT_FALSE(
      command_controller->IsCommandEnabled(IDC_NEW_TOR_CONNECTION_FOR_SITE));
  EXPECT_TRUE(
      command_controller->IsCommandEnabled(IDC_NEW_OFFTHERECORD_WINDOW_TOR));
#endif

  if (syncer::IsSyncAllowedByFlag())
    EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_HNS_SYNC));
  else
    EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_SHOW_HNS_SYNC));

  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_HNS_WALLET));
  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_ADD_NEW_PROFILE));
  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_OPEN_GUEST_PROFILE));
  EXPECT_TRUE(
      command_controller->IsCommandEnabled(IDC_SHOW_HNS_WEBCOMPAT_REPORTER));
}

// Create guest browser and test its hns commands status.
IN_PROC_BROWSER_TEST_F(HnsBrowserCommandControllerTest,
                       HnsCommandsEnableTestGuestWindow) {
  ui_test_utils::BrowserChangeObserver browser_creation_observer(
      nullptr, ui_test_utils::BrowserChangeObserver::ChangeType::kAdded);
  profiles::SwitchToGuestProfile(base::DoNothing());

  Browser* guest_browser = browser_creation_observer.Wait();
  DCHECK(guest_browser);
  EXPECT_TRUE(guest_browser->profile()->IsGuestSession());
  auto* command_controller = guest_browser->command_controller();
  EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_SHOW_HNS_REWARDS));

#if BUILDFLAG(ENABLE_TOR)
  EXPECT_FALSE(
      command_controller->IsCommandEnabled(IDC_NEW_TOR_CONNECTION_FOR_SITE));
  EXPECT_FALSE(
      command_controller->IsCommandEnabled(IDC_NEW_OFFTHERECORD_WINDOW_TOR));
#endif

  EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_SHOW_HNS_SYNC));

  EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_SHOW_HNS_WALLET));
  EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_ADD_NEW_PROFILE));
  EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_OPEN_GUEST_PROFILE));
  EXPECT_TRUE(
      command_controller->IsCommandEnabled(IDC_SHOW_HNS_WEBCOMPAT_REPORTER));
}

// Launch tor window and check its command status.
#if BUILDFLAG(ENABLE_TOR)
IN_PROC_BROWSER_TEST_F(HnsBrowserCommandControllerTest,
                       HnsCommandsEnableTestPrivateTorWindow) {
  ui_test_utils::BrowserChangeObserver tor_browser_creation_observer(
      nullptr, ui_test_utils::BrowserChangeObserver::ChangeType::kAdded);
  hns::NewOffTheRecordWindowTor(browser());
  Browser* tor_browser = tor_browser_creation_observer.Wait();
  DCHECK(tor_browser);
  EXPECT_TRUE(tor_browser->profile()->IsTor());
  auto* command_controller = tor_browser->command_controller();
  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_HNS_REWARDS));

  EXPECT_TRUE(
      command_controller->IsCommandEnabled(IDC_NEW_TOR_CONNECTION_FOR_SITE));
  EXPECT_TRUE(
      command_controller->IsCommandEnabled(IDC_NEW_OFFTHERECORD_WINDOW_TOR));

  if (syncer::IsSyncAllowedByFlag())
    EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_HNS_SYNC));
  else
    EXPECT_FALSE(command_controller->IsCommandEnabled(IDC_SHOW_HNS_SYNC));

  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_SHOW_HNS_WALLET));
  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_ADD_NEW_PROFILE));
  EXPECT_TRUE(command_controller->IsCommandEnabled(IDC_OPEN_GUEST_PROFILE));
  EXPECT_TRUE(
      command_controller->IsCommandEnabled(IDC_SHOW_HNS_WEBCOMPAT_REPORTER));

  // Check tor commands when tor is disabled.
  TorProfileServiceFactory::SetTorDisabled(true);
  command_controller = browser()->command_controller();
  EXPECT_FALSE(
      command_controller->IsCommandEnabled(IDC_NEW_TOR_CONNECTION_FOR_SITE));
  EXPECT_FALSE(
      command_controller->IsCommandEnabled(IDC_NEW_OFFTHERECORD_WINDOW_TOR));
}
#endif

/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_vpn/dns/hns_vpn_dns_observer_service_win.h"

#include <unordered_map>

#include "base/run_loop.h"
#include "base/test/bind.h"
#include "base/test/scoped_feature_list.h"
#include "hns/browser/hns_profile_prefs.h"
#include "hns/browser/hns_vpn/dns/hns_vpn_dns_observer_factory_win.h"
#include "hns/components/hns_vpn/common/hns_vpn_utils.h"
#include "hns/components/hns_vpn/common/features.h"
#include "hns/components/hns_vpn/common/pref_names.h"
#include "chrome/browser/net/secure_dns_config.h"
#include "chrome/browser/net/secure_dns_util.h"
#include "chrome/browser/net/stub_resolver_config_reader.h"
#include "chrome/browser/net/system_network_context_manager.h"
#include "chrome/browser/prefs/browser_prefs.h"
#include "chrome/common/pref_names.h"
#include "chrome/test/base/scoped_testing_local_state.h"
#include "chrome/test/base/testing_browser_process.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/sync_preferences/testing_pref_service_syncable.h"
#include "content/public/test/browser_task_environment.h"
#include "net/dns/public/secure_dns_mode.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace hns_vpn {
namespace {
const char kCustomServersURLs[] =
    "https://server1.com\nhttps://server2.com/{?dns}";
const char kCloudflareDnsProviderURL[] =
    "https://chrome.cloudflare-dns.com/dns-query";
}  // namespace

class HnsVpnDnsObserverServiceUnitTest : public testing::Test {
 public:
  HnsVpnDnsObserverServiceUnitTest() {}

  void SetUp() override {
    RegisterLocalState(local_state_.registry());
    HnsVpnDnsObserverFactory::GetInstance()->RegisterProfilePrefs(
        profile_pref_service_.registry());
    stub_resolver_config_reader_ =
        std::make_unique<StubResolverConfigReader>(&local_state_);
    SystemNetworkContextManager::set_stub_resolver_config_reader_for_testing(
        stub_resolver_config_reader_.get());
    CreateDnsObserverService();
  }

  void CreateDnsObserverService() {
    dns_observer_service_.reset(
        new HnsVpnDnsObserverService(local_state(), pref_service()));
    dns_observer_service_->SetVPNNotificationCallbackForTesting(
        base::DoNothing());
    SetDNSHelperLive(false);
  }
  void SetDNSHelperLive(bool value) {
    dns_observer_service_->SetDNSHelperLiveForTesting(value);
  }
  void ResetDnsObserverService() { dns_observer_service_.reset(); }

  void TearDown() override {
    // HnsVpnDnsObserverService destructor must be called before the task
    // runner is destroyed.
    ResetDnsObserverService();
  }
  void EnableParentalControl(bool value) {
    StubResolverConfigReader* config_reader =
        SystemNetworkContextManager::GetStubResolverConfigReader();
    config_reader->OverrideParentalControlsForTesting(value);
  }
  PrefService* local_state() { return &local_state_; }
  PrefService* pref_service() { return &profile_pref_service_; }

  void FireHnsVPNStateChange(mojom::ConnectionState state) {
    dns_observer_service_->OnConnectionStateChanged(state);
  }

  bool WasVpnNotificationShownForState(mojom::ConnectionState state) {
    bool callback_called = false;
    dns_observer_service_->SetVPNNotificationCallbackForTesting(
        base::BindLambdaForTesting([&]() { callback_called = true; }));
    FireHnsVPNStateChange(state);
    return callback_called;
  }

  void CheckUserNotifiedAndDnsOverridenOnLaunch(
      const std::string& user_dns_mode,
      const std::string& user_servers,
      const std::string& expected_servers,
      bool expected_dialog_shown) {
    ResetDnsObserverService();
    // DNS mode was set to off by user.
    SetDNSMode(user_dns_mode, user_servers);
    // Set vpn config to indicate vpn was enabled when browser closed
    local_state()->SetString(::prefs::kHnsVpnDnsConfig,
                             kCloudflareDnsProviderURL);
    CreateDnsObserverService();
    EXPECT_TRUE(local_state()->GetString(::prefs::kHnsVpnDnsConfig).empty());
    // After launch HnsVPNService will notify observers with actual state.
    // and we expect the dns notification dialog will not be shown as vpn is
    // enabled.
    EXPECT_EQ(
        WasVpnNotificationShownForState(mojom::ConnectionState::CONNECTED),
        expected_dialog_shown);

    // the dns config was overriden.
    ExpectDNSMode(SecureDnsConfig::kModeSecure, expected_servers);
  }

  void SetDNSMode(const std::string& mode, const std::string& doh_providers) {
    local_state()->SetString(::prefs::kDnsOverHttpsTemplates, doh_providers);
    local_state()->SetString(::prefs::kDnsOverHttpsMode, mode);
    SystemNetworkContextManager::GetStubResolverConfigReader()
        ->UpdateNetworkService(false);
  }

  bool WasPolicyNotificationShownForState(mojom::ConnectionState state) {
    bool callback_called = false;
    dns_observer_service_->SetPolicyNotificationCallbackForTesting(
        base::BindLambdaForTesting([&]() { callback_called = true; }));
    FireHnsVPNStateChange(state);
    return callback_called;
  }

  void ExpectDNSMode(const std::string& mode,
                     const std::string& doh_providers) {
    auto dns_config = SystemNetworkContextManager::GetStubResolverConfigReader()
                          ->GetSecureDnsConfiguration(false);
    auto* current_mode = SecureDnsConfig::ModeToString(dns_config.mode());
    auto current_servers = dns_config.doh_servers().ToString();
    EXPECT_EQ(current_mode, mode);
    EXPECT_EQ(current_servers, doh_providers);
  }

  void SetManagedMode(const std::string& value) {
    local_state_.SetManagedPref(::prefs::kDnsOverHttpsMode, base::Value(value));
  }

 private:
  std::unordered_map<std::string, std::string> policy_map_;
  content::BrowserTaskEnvironment task_environment_;
  std::unique_ptr<HnsVpnDnsObserverService> dns_observer_service_;
  sync_preferences::TestingPrefServiceSyncable profile_pref_service_;
  TestingPrefServiceSimple local_state_;
  std::unique_ptr<StubResolverConfigReader> stub_resolver_config_reader_;
};

TEST_F(HnsVpnDnsObserverServiceUnitTest, OverrideDohConfigForOffMode) {
  // Browser DoH mode off -> override browser config and enable vpn
  local_state()->ClearPref(::prefs::kHnsVpnDnsConfig);
  SetDNSMode(SecureDnsConfig::kModeOff, "");
  FireHnsVPNStateChange(mojom::ConnectionState::CONNECTING);
  ExpectDNSMode(SecureDnsConfig::kModeOff, "");
  FireHnsVPNStateChange(mojom::ConnectionState::CONNECTED);
  ExpectDNSMode(SecureDnsConfig::kModeSecure, kCloudflareDnsProviderURL);
  EXPECT_FALSE(local_state()->GetString(::prefs::kHnsVpnDnsConfig).empty());
  FireHnsVPNStateChange(mojom::ConnectionState::DISCONNECTING);
  ExpectDNSMode(SecureDnsConfig::kModeSecure, kCloudflareDnsProviderURL);
  FireHnsVPNStateChange(mojom::ConnectionState::DISCONNECTED);
  ExpectDNSMode(SecureDnsConfig::kModeOff, "");
  EXPECT_TRUE(local_state()->GetString(::prefs::kHnsVpnDnsConfig).empty());
}

TEST_F(HnsVpnDnsObserverServiceUnitTest, OverrideDohConfigForAutomaticMode) {
  // Browser DoH mode automatic -> override browser config and enable vpn
  SetDNSMode(SecureDnsConfig::kModeAutomatic, "");
  FireHnsVPNStateChange(mojom::ConnectionState::CONNECTING);
  ExpectDNSMode(SecureDnsConfig::kModeAutomatic, "");
  FireHnsVPNStateChange(mojom::ConnectionState::CONNECTED);
  ExpectDNSMode(SecureDnsConfig::kModeSecure, kCloudflareDnsProviderURL);
  EXPECT_FALSE(local_state()->GetString(::prefs::kHnsVpnDnsConfig).empty());
  FireHnsVPNStateChange(mojom::ConnectionState::DISCONNECTING);
  ExpectDNSMode(SecureDnsConfig::kModeSecure, kCloudflareDnsProviderURL);
  FireHnsVPNStateChange(mojom::ConnectionState::DISCONNECTED);
  ExpectDNSMode(SecureDnsConfig::kModeAutomatic, "");
  EXPECT_TRUE(local_state()->GetString(::prefs::kHnsVpnDnsConfig).empty());
}

TEST_F(HnsVpnDnsObserverServiceUnitTest, OverrideDohConfigForSecureMode) {
  // Browser DoH mode secure -> override browser config and enable vpn
  SetDNSMode(SecureDnsConfig::kModeSecure, kCloudflareDnsProviderURL);
  FireHnsVPNStateChange(mojom::ConnectionState::CONNECTING);
  ExpectDNSMode(SecureDnsConfig::kModeSecure, kCloudflareDnsProviderURL);
  FireHnsVPNStateChange(mojom::ConnectionState::CONNECTED);
  ExpectDNSMode(SecureDnsConfig::kModeSecure, kCloudflareDnsProviderURL);
  EXPECT_FALSE(local_state()->GetString(::prefs::kHnsVpnDnsConfig).empty());
  FireHnsVPNStateChange(mojom::ConnectionState::DISCONNECTING);
  ExpectDNSMode(SecureDnsConfig::kModeSecure, kCloudflareDnsProviderURL);
  FireHnsVPNStateChange(mojom::ConnectionState::DISCONNECTED);
  ExpectDNSMode(SecureDnsConfig::kModeSecure, kCloudflareDnsProviderURL);
  EXPECT_TRUE(local_state()->GetString(::prefs::kHnsVpnDnsConfig).empty());
}

TEST_F(HnsVpnDnsObserverServiceUnitTest,
       OverrideDohConfigForAutomaticModeWithValidCustomServers) {
  // Browser DoH mode automatic with custom servers
  // -> we override browser config and enable vpn
  SetDNSMode(SecureDnsConfig::kModeAutomatic, kCustomServersURLs);
  FireHnsVPNStateChange(mojom::ConnectionState::CONNECTING);
  ExpectDNSMode(SecureDnsConfig::kModeAutomatic, kCustomServersURLs);
  FireHnsVPNStateChange(mojom::ConnectionState::CONNECTED);
  ExpectDNSMode(SecureDnsConfig::kModeSecure, kCloudflareDnsProviderURL);
  EXPECT_FALSE(local_state()->GetString(::prefs::kHnsVpnDnsConfig).empty());
  FireHnsVPNStateChange(mojom::ConnectionState::DISCONNECTING);
  ExpectDNSMode(SecureDnsConfig::kModeSecure, kCloudflareDnsProviderURL);
  FireHnsVPNStateChange(mojom::ConnectionState::DISCONNECTED);
  ExpectDNSMode(SecureDnsConfig::kModeAutomatic, kCustomServersURLs);
  EXPECT_TRUE(local_state()->GetString(::prefs::kHnsVpnDnsConfig).empty());
}

TEST_F(HnsVpnDnsObserverServiceUnitTest,
       OverrideDohConfigForAutomaticModeWithBrokenCustomServers) {
  // Browser DoH mode automatic with broken custom servers
  // -> override browser config and enable vpn
  SetDNSMode(SecureDnsConfig::kModeAutomatic, std::string());
  FireHnsVPNStateChange(mojom::ConnectionState::CONNECTING);
  ExpectDNSMode(SecureDnsConfig::kModeAutomatic, std::string());
  FireHnsVPNStateChange(mojom::ConnectionState::CONNECTED);
  ExpectDNSMode(SecureDnsConfig::kModeSecure, kCloudflareDnsProviderURL);
  EXPECT_FALSE(local_state()->GetString(::prefs::kHnsVpnDnsConfig).empty());
  FireHnsVPNStateChange(mojom::ConnectionState::DISCONNECTING);
  ExpectDNSMode(SecureDnsConfig::kModeSecure, kCloudflareDnsProviderURL);
  FireHnsVPNStateChange(mojom::ConnectionState::DISCONNECTED);
  ExpectDNSMode(SecureDnsConfig::kModeAutomatic, std::string());
  EXPECT_TRUE(local_state()->GetString(::prefs::kHnsVpnDnsConfig).empty());
}

TEST_F(HnsVpnDnsObserverServiceUnitTest,
       OverrideDohConfigForSecureModeWithCustomServers) {
  // Browser DoH mode secure with custom servers
  // -> override browser config and enable vpn
  SetDNSMode(SecureDnsConfig::kModeSecure, kCustomServersURLs);
  FireHnsVPNStateChange(mojom::ConnectionState::CONNECTING);
  ExpectDNSMode(SecureDnsConfig::kModeSecure, kCustomServersURLs);
  FireHnsVPNStateChange(mojom::ConnectionState::CONNECTED);
  ExpectDNSMode(SecureDnsConfig::kModeSecure, kCustomServersURLs);
  EXPECT_FALSE(local_state()->GetString(::prefs::kHnsVpnDnsConfig).empty());
  FireHnsVPNStateChange(mojom::ConnectionState::DISCONNECTING);
  ExpectDNSMode(SecureDnsConfig::kModeSecure, kCustomServersURLs);
  FireHnsVPNStateChange(mojom::ConnectionState::DISCONNECTED);
  ExpectDNSMode(SecureDnsConfig::kModeSecure, kCustomServersURLs);
  EXPECT_TRUE(local_state()->GetString(::prefs::kHnsVpnDnsConfig).empty());
}

TEST_F(HnsVpnDnsObserverServiceUnitTest,
       DoNotOverrideDoHConfigWithPolicyOff) {
  SetManagedMode(SecureDnsConfig::kModeOff);

  SetDNSMode(SecureDnsConfig::kModeOff, "");
  EXPECT_FALSE(
      WasPolicyNotificationShownForState(mojom::ConnectionState::CONNECTING));
  ExpectDNSMode(SecureDnsConfig::kModeOff, "");
  EXPECT_TRUE(
      WasPolicyNotificationShownForState(mojom::ConnectionState::CONNECTED));
  ExpectDNSMode(SecureDnsConfig::kModeOff, "");
  EXPECT_FALSE(WasPolicyNotificationShownForState(
      mojom::ConnectionState::DISCONNECTING));
  ExpectDNSMode(SecureDnsConfig::kModeOff, "");
  EXPECT_FALSE(
      WasPolicyNotificationShownForState(mojom::ConnectionState::DISCONNECTED));
  ExpectDNSMode(SecureDnsConfig::kModeOff, "");
}

TEST_F(HnsVpnDnsObserverServiceUnitTest,
       DoNotOverrideDoHConfigWithPolicyAutomatic) {
  SetManagedMode(SecureDnsConfig::kModeAutomatic);

  SetDNSMode(SecureDnsConfig::kModeAutomatic, "");
  EXPECT_FALSE(
      WasPolicyNotificationShownForState(mojom::ConnectionState::CONNECTING));
  ExpectDNSMode(SecureDnsConfig::kModeAutomatic, "");
  EXPECT_TRUE(
      WasPolicyNotificationShownForState(mojom::ConnectionState::CONNECTED));
  ExpectDNSMode(SecureDnsConfig::kModeAutomatic, "");
  EXPECT_FALSE(WasPolicyNotificationShownForState(
      mojom::ConnectionState::DISCONNECTING));
  ExpectDNSMode(SecureDnsConfig::kModeAutomatic, "");
  EXPECT_FALSE(
      WasPolicyNotificationShownForState(mojom::ConnectionState::DISCONNECTED));
  ExpectDNSMode(SecureDnsConfig::kModeAutomatic, "");
}

TEST_F(HnsVpnDnsObserverServiceUnitTest,
       DoNotOverrideDoHConfigWithPolicySecure) {
  SetManagedMode(SecureDnsConfig::kModeSecure);
  SetDNSMode(SecureDnsConfig::kModeSecure, "");
  EXPECT_FALSE(
      WasPolicyNotificationShownForState(mojom::ConnectionState::CONNECTING));
  ExpectDNSMode(SecureDnsConfig::kModeSecure, "");
  EXPECT_FALSE(
      WasPolicyNotificationShownForState(mojom::ConnectionState::CONNECTED));
  ExpectDNSMode(SecureDnsConfig::kModeSecure, "");
  EXPECT_FALSE(WasPolicyNotificationShownForState(
      mojom::ConnectionState::DISCONNECTING));
  ExpectDNSMode(SecureDnsConfig::kModeSecure, "");
  EXPECT_FALSE(
      WasPolicyNotificationShownForState(mojom::ConnectionState::DISCONNECTED));
  ExpectDNSMode(SecureDnsConfig::kModeSecure, "");
}

TEST_F(HnsVpnDnsObserverServiceUnitTest,
       DoNotOverrideDoHConfigWithPolicySecureAndCustomServers) {
  SetDNSMode(SecureDnsConfig::kModeSecure, kCustomServersURLs);
  SetManagedMode(SecureDnsConfig::kModeSecure);
  EXPECT_FALSE(
      WasPolicyNotificationShownForState(mojom::ConnectionState::CONNECTING));
  ExpectDNSMode(SecureDnsConfig::kModeSecure, kCustomServersURLs);
  EXPECT_FALSE(
      WasPolicyNotificationShownForState(mojom::ConnectionState::CONNECTED));
  ExpectDNSMode(SecureDnsConfig::kModeSecure, kCustomServersURLs);
  EXPECT_FALSE(WasPolicyNotificationShownForState(
      mojom::ConnectionState::DISCONNECTING));
  ExpectDNSMode(SecureDnsConfig::kModeSecure, kCustomServersURLs);
  EXPECT_FALSE(
      WasPolicyNotificationShownForState(mojom::ConnectionState::DISCONNECTED));
  ExpectDNSMode(SecureDnsConfig::kModeSecure, kCustomServersURLs);
}

TEST_F(HnsVpnDnsObserverServiceUnitTest,
       DoNotOverrideDoHConfigWithParentalControlEnabled) {
  SetDNSMode(SecureDnsConfig::kModeSecure, "");
  EnableParentalControl(true);
  EXPECT_TRUE(
      WasPolicyNotificationShownForState(mojom::ConnectionState::CONNECTED));
  ExpectDNSMode(SecureDnsConfig::kModeOff, "");
  EXPECT_FALSE(
      WasPolicyNotificationShownForState(mojom::ConnectionState::DISCONNECTED));
  ExpectDNSMode(SecureDnsConfig::kModeOff, "");

  EnableParentalControl(false);
  ExpectDNSMode(SecureDnsConfig::kModeSecure, "");
}

TEST_F(HnsVpnDnsObserverServiceUnitTest, DoNotShowPolicyDialogIfUserSkipped) {
  // Do not show dialog option enabled
  SetManagedMode(SecureDnsConfig::kModeOff);
  pref_service()->SetBoolean(prefs::kHnsVpnShowDNSPolicyWarningDialog, false);
  SetDNSMode(SecureDnsConfig::kModeOff, "");
  EXPECT_FALSE(
      WasPolicyNotificationShownForState(mojom::ConnectionState::CONNECTED));
}

TEST_F(HnsVpnDnsObserverServiceUnitTest, DnsOverridenOnLaunchIfVPNEnabled) {
  CheckUserNotifiedAndDnsOverridenOnLaunch(SecureDnsConfig::kModeOff,
                                           std::string(),
                                           kCloudflareDnsProviderURL, true);
  CheckUserNotifiedAndDnsOverridenOnLaunch(SecureDnsConfig::kModeAutomatic,
                                           std::string(),
                                           kCloudflareDnsProviderURL, true);
  CheckUserNotifiedAndDnsOverridenOnLaunch(SecureDnsConfig::kModeSecure, "", "",
                                           false);
}

TEST_F(HnsVpnDnsObserverServiceUnitTest,
       VPNConnectedOnBrowserShutDownAndDisconnectedOnStart) {
  ResetDnsObserverService();
  // DNS mode was set to off by user.
  SetDNSMode(SecureDnsConfig::kModeOff, "");
  // Set vpn config to indicate vpn was enabled when browser closed.
  local_state()->SetString(::prefs::kHnsVpnDnsConfig,
                           kCloudflareDnsProviderURL);
  CreateDnsObserverService();
  // Before VPN service initialization we have vpn overriden.
  ExpectDNSMode(SecureDnsConfig::kModeOff, "");
  EXPECT_TRUE(local_state()->GetString(::prefs::kHnsVpnDnsConfig).empty());
  // On launch vpn service notifies it is disconnected.
  FireHnsVPNStateChange(mojom::ConnectionState::DISCONNECTED);
  // Do not override anymore as vpn is disconnected.
  ExpectDNSMode(SecureDnsConfig::kModeOff, "");
  EXPECT_TRUE(local_state()->GetString(::prefs::kHnsVpnDnsConfig).empty());
}

TEST_F(HnsVpnDnsObserverServiceUnitTest, FeatureDisabledWhenVPNConnected) {
  // DNS mode was set to off by user and the vpn dns observer feature
  // is enabled by default.
  SetDNSMode(SecureDnsConfig::kModeOff, "");
  FireHnsVPNStateChange(mojom::ConnectionState::CONNECTED);
  ExpectDNSMode(SecureDnsConfig::kModeSecure, kCloudflareDnsProviderURL);

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndDisableFeature(
      hns_vpn::features::kHnsVPNDnsProtection);
  // Do not override anymore because the feature is disabled.
  ExpectDNSMode(SecureDnsConfig::kModeOff, "");
  EXPECT_FALSE(local_state()->GetString(::prefs::kHnsVpnDnsConfig).empty());
}

TEST_F(HnsVpnDnsObserverServiceUnitTest, HelperServerLiveWhenVPNConnected) {
  SetDNSHelperLive(true);
  // DNS mode was set to off by user and the vpn dns observer feature
  // is enabled by default.
  SetDNSMode(SecureDnsConfig::kModeOff, "");
  FireHnsVPNStateChange(mojom::ConnectionState::CONNECTED);
  // DNS mode was not overriden.
  ExpectDNSMode(SecureDnsConfig::kModeOff, "");
}

}  // namespace hns_vpn

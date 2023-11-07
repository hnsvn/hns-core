/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_vpn/browser/connection/wireguard/hns_vpn_wireguard_connection_api_base.h"

#include <memory>
#include "hns/components/hns_vpn/common/hns_vpn_data_types.h"
#include "hns/components/hns_vpn/common/hns_vpn_utils.h"
#include "hns/components/hns_vpn/common/pref_names.h"
#include "components/sync_preferences/testing_pref_service_syncable.h"
#include "content/public/test/browser_task_environment.h"
#include "services/network/public/cpp/weak_wrapper_shared_url_loader_factory.h"
#include "services/network/test/test_url_loader_factory.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace hns_vpn {

namespace {
class HnsVPNWireguardConnectionAPISim
    : public HnsVPNWireguardConnectionAPIBase {
 public:
  HnsVPNWireguardConnectionAPISim(
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
      PrefService* local_prefs)
      : HnsVPNWireguardConnectionAPIBase(url_loader_factory, local_prefs) {}

  ~HnsVPNWireguardConnectionAPISim() override {}

  HnsVPNWireguardConnectionAPISim(const HnsVPNWireguardConnectionAPISim&) =
      delete;
  HnsVPNWireguardConnectionAPISim& operator=(
      const HnsVPNWireguardConnectionAPISim&) = delete;

  void Disconnect() override {}
  void CheckConnection() override {}
  void RequestNewProfileCredentials() override {}
  void PlatformConnectImpl(
      const wireguard::WireguardProfileCredentials& credentials) override {}
};
}  // namespace

class HnsVPNWireguardConnectionAPIUnitTest : public testing::Test {
 public:
  HnsVPNWireguardConnectionAPIUnitTest()
      : task_environment_(base::test::TaskEnvironment::TimeSource::MOCK_TIME) {}

  void SetUp() override {
    hns_vpn::RegisterLocalStatePrefs(local_pref_service_.registry());
    connection_api_ = std::make_unique<HnsVPNWireguardConnectionAPISim>(
        base::MakeRefCounted<network::WeakWrapperSharedURLLoaderFactory>(
            &url_loader_factory_),
        local_state());
  }

  HnsVPNWireguardConnectionAPIBase* GetHnsVPNWireguardConnectionAPIBase()
      const {
    return static_cast<HnsVPNWireguardConnectionAPIBase*>(
        connection_api_.get());
  }
  PrefService* local_state() { return &local_pref_service_; }

  HnsVPNOSConnectionAPI* GetConnectionAPI() { return connection_api_.get(); }

 protected:
  TestingPrefServiceSimple local_pref_service_;
  network::TestURLLoaderFactory url_loader_factory_;
  content::BrowserTaskEnvironment task_environment_;
  std::unique_ptr<HnsVPNOSConnectionAPI> connection_api_;
};

TEST_F(HnsVPNWireguardConnectionAPIUnitTest, SetSelectedRegion) {
  local_state()->SetString(prefs::kHnsVPNWireguardProfileCredentials,
                           "region-a");
  GetHnsVPNWireguardConnectionAPIBase()->hostname_ =
      std::make_unique<Hostname>();
  GetHnsVPNWireguardConnectionAPIBase()->hostname_->hostname = "test";
  GetHnsVPNWireguardConnectionAPIBase()->SetSelectedRegion("region-b");
  EXPECT_TRUE(local_state()
                  ->GetString(prefs::kHnsVPNWireguardProfileCredentials)
                  .empty());
  EXPECT_EQ(GetHnsVPNWireguardConnectionAPIBase()->hostname_.get(), nullptr);
}

}  // namespace hns_vpn

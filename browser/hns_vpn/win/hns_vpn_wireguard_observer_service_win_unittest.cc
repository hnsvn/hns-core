/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_vpn/win/hns_vpn_wireguard_observer_service_win.h"

#include <memory>

#include "base/run_loop.h"
#include "base/test/bind.h"
#include "content/public/test/browser_task_environment.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace hns_vpn {

class HnsVpnWireguardObserverServiceUnitTest : public testing::Test {
 public:
  HnsVpnWireguardObserverServiceUnitTest() {}

  void SetUp() override { CreateWireguardObserverService(); }

  void CreateWireguardObserverService() {
    wireguard_observer_service_.reset(new HnsVpnWireguardObserverService());
    wireguard_observer_service_->SetDialogCallbackForTesting(base::DoNothing());
  }

  void ResetWireguardObserverService() { wireguard_observer_service_.reset(); }

  void TearDown() override {
    // HnsVpnWireguardObserverService destructor must be called before
    // the task runner is destroyed.
    ResetWireguardObserverService();
  }

  void FireHnsVPNStateChange(mojom::ConnectionState state) {
    wireguard_observer_service_->OnConnectionStateChanged(state);
  }

  bool WasVpnFallbackShownForState(mojom::ConnectionState state,
                                   bool fallback) {
    bool callback_called = false;
    wireguard_observer_service_->SetDialogCallbackForTesting(
        base::BindLambdaForTesting([&]() { callback_called = true; }));
    wireguard_observer_service_->SetFallbackForTesting(fallback);
    FireHnsVPNStateChange(state);
    return callback_called;
  }

 private:
  content::BrowserTaskEnvironment task_environment_;
  std::unique_ptr<HnsVpnWireguardObserverService> wireguard_observer_service_;
};

TEST_F(HnsVpnWireguardObserverServiceUnitTest, FallbackToIKEv2) {
  EXPECT_FALSE(
      WasVpnFallbackShownForState(mojom::ConnectionState::CONNECTING, true));
  EXPECT_FALSE(
      WasVpnFallbackShownForState(mojom::ConnectionState::CONNECTED, true));
  EXPECT_FALSE(
      WasVpnFallbackShownForState(mojom::ConnectionState::DISCONNECTING, true));

  EXPECT_TRUE(
      WasVpnFallbackShownForState(mojom::ConnectionState::DISCONNECTED, true));
  EXPECT_TRUE(WasVpnFallbackShownForState(
      mojom::ConnectionState::CONNECT_FAILED, true));

  EXPECT_FALSE(
      WasVpnFallbackShownForState(mojom::ConnectionState::DISCONNECTED, false));
  EXPECT_FALSE(WasVpnFallbackShownForState(
      mojom::ConnectionState::CONNECT_FAILED, false));
}

}  // namespace hns_vpn

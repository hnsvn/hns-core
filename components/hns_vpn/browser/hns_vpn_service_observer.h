/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_BROWSER_HNS_VPN_SERVICE_OBSERVER_H_
#define HNS_COMPONENTS_HNS_VPN_BROWSER_HNS_VPN_SERVICE_OBSERVER_H_

#include <string>

#include "hns/components/hns_vpn/common/mojom/hns_vpn.mojom.h"
#include "build/build_config.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_vpn {

class HnsVpnService;

class HnsVPNServiceObserver : public mojom::ServiceObserver {
 public:
  HnsVPNServiceObserver();
  ~HnsVPNServiceObserver() override;
  HnsVPNServiceObserver(const HnsVPNServiceObserver&) = delete;
  HnsVPNServiceObserver& operator=(const HnsVPNServiceObserver&) = delete;

  void Observe(HnsVpnService* service);

  // mojom::ServiceObserver overrides:
  void OnPurchasedStateChanged(
      mojom::PurchasedState state,
      const absl::optional<std::string>& description) override {}
#if !BUILDFLAG(IS_ANDROID)
  void OnConnectionStateChanged(mojom::ConnectionState state) override {}
  void OnSelectedRegionChanged(mojom::RegionPtr region) override {}
#endif  // !BUILDFLAG(IS_ANDROID)

 private:
  mojo::Receiver<mojom::ServiceObserver> receiver_{this};
};

}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_BROWSER_HNS_VPN_SERVICE_OBSERVER_H_

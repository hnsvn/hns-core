/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_OBSERVER_SERVICE_WIN_H_
#define HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_OBSERVER_SERVICE_WIN_H_

#include <utility>

#include "hns/components/hns_vpn/browser/hns_vpn_service_observer.h"
#include "components/keyed_service/core/keyed_service.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_vpn {

class HnsVpnWireguardObserverService
    : public hns_vpn::HnsVPNServiceObserver,
      public KeyedService {
 public:
  HnsVpnWireguardObserverService();
  ~HnsVpnWireguardObserverService() override;
  HnsVpnWireguardObserverService(const HnsVpnWireguardObserverService&) =
      delete;
  HnsVpnWireguardObserverService operator=(
      const HnsVpnWireguardObserverService&) = delete;

  // hns_vpn::HnsVPNServiceObserver
  void OnConnectionStateChanged(
      hns_vpn::mojom::ConnectionState state) override;

 private:
  friend class HnsVpnWireguardObserverServiceUnitTest;

  void SetDialogCallbackForTesting(base::RepeatingClosure callback) {
    dialog_callback_ = std::move(callback);
  }

  void SetFallbackForTesting(bool should_fallback_for_testing) {
    should_fallback_for_testing_ = should_fallback_for_testing;
  }

  void ShowFallbackDialog();
  bool ShouldShowFallbackDialog() const;

  absl::optional<bool> should_fallback_for_testing_;
  base::RepeatingClosure dialog_callback_;
};

}  // namespace hns_vpn

#endif  // HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_OBSERVER_SERVICE_WIN_H_

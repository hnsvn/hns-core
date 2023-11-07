/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_vpn/win/hns_vpn_wireguard_observer_service_win.h"

#include "hns/browser/ui/browser_dialogs.h"
#include "hns/components/hns_vpn/common/wireguard/win/storage_utils.h"

namespace hns_vpn {

HnsVpnWireguardObserverService::HnsVpnWireguardObserverService() = default;

HnsVpnWireguardObserverService::~HnsVpnWireguardObserverService() = default;

void HnsVpnWireguardObserverService::ShowFallbackDialog() {
  if (dialog_callback_) {
    dialog_callback_.Run();
    return;
  }
  hns::ShowHnsVpnIKEv2FallbackDialog();
}

void HnsVpnWireguardObserverService::OnConnectionStateChanged(
    hns_vpn::mojom::ConnectionState state) {
  if (state == hns_vpn::mojom::ConnectionState::DISCONNECTED ||
      state == hns_vpn::mojom::ConnectionState::CONNECT_FAILED) {
    if (ShouldShowFallbackDialog()) {
      ShowFallbackDialog();
    }
  }
}

bool HnsVpnWireguardObserverService::ShouldShowFallbackDialog() const {
  if (should_fallback_for_testing_.has_value()) {
    return should_fallback_for_testing_.value();
  }

  return ShouldFallbackToIKEv2();
}

}  // namespace hns_vpn

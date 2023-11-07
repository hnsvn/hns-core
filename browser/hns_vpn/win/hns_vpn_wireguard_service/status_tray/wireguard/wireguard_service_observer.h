/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_SERVICE_STATUS_TRAY_WIREGUARD_WIREGUARD_SERVICE_OBSERVER_H_
#define HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_SERVICE_STATUS_TRAY_WIREGUARD_WIREGUARD_SERVICE_OBSERVER_H_

#include <memory>
#include <string>

#include "base/memory/weak_ptr.h"
#include "hns/components/hns_vpn/common/win/hns_windows_service_watcher.h"

namespace hns_vpn {
namespace wireguard {

class WireguardServiceObserver {
 public:
  WireguardServiceObserver();

  WireguardServiceObserver(const WireguardServiceObserver&) = delete;
  WireguardServiceObserver& operator=(const WireguardServiceObserver&) = delete;

  virtual ~WireguardServiceObserver();

  virtual void OnWireguardServiceStateChanged(int mask) = 0;

  void SubscribeForWireguardNotifications(const std::wstring& name);
  bool IsWireguardObserverActive() const;
  void StopWireguardObserver();

 private:
  std::unique_ptr<hns::ServiceWatcher> service_watcher_;
  base::WeakPtrFactory<WireguardServiceObserver> weak_factory_{this};
};

}  // namespace wireguard
}  // namespace hns_vpn

#endif  // HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_SERVICE_STATUS_TRAY_WIREGUARD_WIREGUARD_SERVICE_OBSERVER_H_

/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_VPN_DNS_HNS_VPN_DNS_OBSERVER_SERVICE_WIN_H_
#define HNS_BROWSER_HNS_VPN_DNS_HNS_VPN_DNS_OBSERVER_SERVICE_WIN_H_

#include <memory>
#include <string>
#include <utility>

#include "base/memory/weak_ptr.h"
#include "hns/components/hns_vpn/browser/hns_vpn_service_observer.h"
#include "hns/components/hns_vpn/common/win/hns_windows_service_watcher.h"
#include "components/keyed_service/core/keyed_service.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

class PrefService;

namespace hns_vpn {

class HnsVpnDnsObserverService : public hns_vpn::HnsVPNServiceObserver,
                                   public KeyedService {
 public:
  explicit HnsVpnDnsObserverService(PrefService* local_state,
                                      PrefService* profile_prefs);
  ~HnsVpnDnsObserverService() override;
  HnsVpnDnsObserverService(const HnsVpnDnsObserverService&) = delete;
  HnsVpnDnsObserverService operator=(const HnsVpnDnsObserverService&) =
      delete;

  // hns_vpn::HnsVPNServiceObserver
  void OnConnectionStateChanged(
      hns_vpn::mojom::ConnectionState state) override;

  void SetPolicyNotificationCallbackForTesting(base::OnceClosure callback) {
    policy_callback_ = std::move(callback);
  }

  void SetVPNNotificationCallbackForTesting(base::RepeatingClosure callback) {
    dialog_callback_ = std::move(callback);
  }
  void SetDNSHelperLiveForTesting(bool value) {
    dns_helper_live_for_testing_ = value;
  }
  bool IsVPNConnected() const;

 private:
  friend class HnsVpnDnsObserverServiceUnitTest;

  void OnServiceStopped(int mask);
  void RunServiceWatcher();
  void OnPrefChanged();
  bool IsDNSHelperLive();
  void LockDNS();
  void UnlockDNS();
  void ShowPolicyWarningMessage();
  void ShowVpnDnsSettingsNotificationDialog();
  void OnDnsModePrefChanged();

  void OnCheckIfServiceStarted();

  absl::optional<hns_vpn::mojom::ConnectionState> connection_state_;
  std::unique_ptr<hns::ServiceWatcher> service_watcher_;
  absl::optional<bool> dns_helper_live_for_testing_;
  base::OnceClosure policy_callback_;
  base::RepeatingClosure dialog_callback_;
  bool skip_notification_dialog_for_testing_ = false;
  raw_ptr<PrefService> local_state_;
  raw_ptr<PrefService> profile_prefs_;
  base::WeakPtrFactory<HnsVpnDnsObserverService> weak_ptr_factory_{this};
};

}  // namespace hns_vpn

#endif  // HNS_BROWSER_HNS_VPN_DNS_HNS_VPN_DNS_OBSERVER_SERVICE_WIN_H_

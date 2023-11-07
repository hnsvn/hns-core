/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_SERVICE_SERVICE_HNS_WIREGUARD_MANAGER_H_
#define HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_SERVICE_SERVICE_HNS_WIREGUARD_MANAGER_H_

#include <wrl/implements.h>
#include <wrl/module.h>

#include "base/win/windows_types.h"
#include "hns/components/hns_vpn/common/wireguard/win/hns_wireguard_manager_idl.h"

namespace hns_vpn {

class HnsWireguardManager
    : public Microsoft::WRL::RuntimeClass<
          Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>,
          IHnsVpnWireguardManager> {
 public:
  HnsWireguardManager() = default;

  HnsWireguardManager(const HnsWireguardManager&) = delete;
  HnsWireguardManager& operator=(const HnsWireguardManager&) = delete;

  IFACEMETHODIMP EnableVpn(const wchar_t* config, DWORD* last_error) override;
  IFACEMETHODIMP DisableVpn(DWORD* last_error) override;
  IFACEMETHODIMP GenerateKeypair(BSTR* public_key,
                                 BSTR* private_key,
                                 DWORD* last_error) override;

 private:
  ~HnsWireguardManager() override = default;
};

}  // namespace hns_vpn

#endif  // HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_SERVICE_SERVICE_HNS_WIREGUARD_MANAGER_H_

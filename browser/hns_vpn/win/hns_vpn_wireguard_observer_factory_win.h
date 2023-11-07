/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_OBSERVER_FACTORY_WIN_H_
#define HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_OBSERVER_FACTORY_WIN_H_

#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

namespace content {
class BrowserContext;
}  // namespace content

namespace hns_vpn {

class HnsVpnWireguardObserverService;

class HnsVpnWireguardObserverFactory
    : public BrowserContextKeyedServiceFactory {
 public:
  HnsVpnWireguardObserverFactory(const HnsVpnWireguardObserverFactory&) =
      delete;
  HnsVpnWireguardObserverFactory& operator=(
      const HnsVpnWireguardObserverFactory&) = delete;

  static HnsVpnWireguardObserverFactory* GetInstance();
  static HnsVpnWireguardObserverService* GetServiceForContext(
      content::BrowserContext* context);

 private:
  friend base::NoDestructor<HnsVpnWireguardObserverFactory>;

  HnsVpnWireguardObserverFactory();
  ~HnsVpnWireguardObserverFactory() override;

  // BrowserContextKeyedServiceFactory overrides:
  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const override;
};

}  // namespace hns_vpn

#endif  // HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_OBSERVER_FACTORY_WIN_H_

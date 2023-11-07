/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_VPN_DNS_HNS_VPN_DNS_OBSERVER_FACTORY_WIN_H_
#define HNS_BROWSER_HNS_VPN_DNS_HNS_VPN_DNS_OBSERVER_FACTORY_WIN_H_

#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

namespace content {
class BrowserContext;
}  // namespace content

namespace user_prefs {
class PrefRegistrySyncable;
}  // namespace user_prefs

namespace hns_vpn {

class HnsVpnDnsObserverService;

class HnsVpnDnsObserverFactory : public BrowserContextKeyedServiceFactory {
 public:
  HnsVpnDnsObserverFactory(const HnsVpnDnsObserverFactory&) = delete;
  HnsVpnDnsObserverFactory& operator=(const HnsVpnDnsObserverFactory&) =
      delete;

  static HnsVpnDnsObserverFactory* GetInstance();
  static HnsVpnDnsObserverService* GetServiceForContext(
      content::BrowserContext* context);
  void RegisterProfilePrefs(
      user_prefs::PrefRegistrySyncable* registry) override;

 private:
  friend base::NoDestructor<HnsVpnDnsObserverFactory>;

  HnsVpnDnsObserverFactory();
  ~HnsVpnDnsObserverFactory() override;

  // BrowserContextKeyedServiceFactory overrides:
  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const override;
};

}  // namespace hns_vpn

#endif  // HNS_BROWSER_HNS_VPN_DNS_HNS_VPN_DNS_OBSERVER_FACTORY_WIN_H_

/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_VPN_HNS_VPN_SERVICE_FACTORY_H_
#define HNS_BROWSER_HNS_VPN_HNS_VPN_SERVICE_FACTORY_H_

#include "hns/components/hns_vpn/common/mojom/hns_vpn.mojom.h"
#include "build/build_config.h"
#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

class Profile;

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

namespace hns_vpn {

class HnsVpnService;

class HnsVpnServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  static HnsVpnService* GetForProfile(Profile* profile);
  static HnsVpnServiceFactory* GetInstance();

  HnsVpnServiceFactory(const HnsVpnServiceFactory&) = delete;
  HnsVpnServiceFactory& operator=(const HnsVpnServiceFactory&) = delete;

  static void BindForContext(
      content::BrowserContext* context,
      mojo::PendingReceiver<hns_vpn::mojom::ServiceHandler> receiver);

 private:
  friend base::NoDestructor<HnsVpnServiceFactory>;

  HnsVpnServiceFactory();
  ~HnsVpnServiceFactory() override;

  // BrowserContextKeyedServiceFactory overrides:
  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const override;
  void RegisterProfilePrefs(
      user_prefs::PrefRegistrySyncable* registry) override;
};

}  // namespace hns_vpn

#endif  // HNS_BROWSER_HNS_VPN_HNS_VPN_SERVICE_FACTORY_H_

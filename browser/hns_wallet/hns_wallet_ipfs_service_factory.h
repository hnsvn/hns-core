/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_WALLET_HNS_WALLET_IPFS_SERVICE_FACTORY_H_
#define HNS_BROWSER_HNS_WALLET_HNS_WALLET_IPFS_SERVICE_FACTORY_H_

#include "hns/components/hns_wallet/browser/hns_wallet_ipfs_service.h"

#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "components/keyed_service/content/browser_context_keyed_service_factory.h"
#include "components/keyed_service/core/keyed_service.h"
#include "content/public/browser/browser_context.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

namespace hns_wallet {

class HnsWalletIpfsServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  HnsWalletIpfsServiceFactory(const HnsWalletIpfsServiceFactory&) = delete;
  HnsWalletIpfsServiceFactory& operator=(
      const HnsWalletIpfsServiceFactory&) = delete;

  static mojo::PendingRemote<mojom::IpfsService> GetForContext(
      content::BrowserContext* context);
  static HnsWalletIpfsService* GetServiceForContext(
      content::BrowserContext* context);
  static HnsWalletIpfsServiceFactory* GetInstance();
  static void BindForContext(
      content::BrowserContext* context,
      mojo::PendingReceiver<mojom::IpfsService> receiver);

 private:
  friend base::NoDestructor<HnsWalletIpfsServiceFactory>;

  HnsWalletIpfsServiceFactory();
  ~HnsWalletIpfsServiceFactory() override;

  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const override;
  content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const override;
};

}  // namespace hns_wallet

#endif  // HNS_BROWSER_HNS_WALLET_HNS_WALLET_IPFS_SERVICE_FACTORY_H_

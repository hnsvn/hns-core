/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_wallet/hns_wallet_ipfs_service_factory.h"

#include <utility>

#include "base/no_destructor.h"
#include "hns/browser/hns_wallet/hns_wallet_context_utils.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/incognito_helpers.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"
#include "components/user_prefs/user_prefs.h"
#include "content/public/browser/storage_partition.h"

namespace hns_wallet {

// static
HnsWalletIpfsServiceFactory* HnsWalletIpfsServiceFactory::GetInstance() {
  static base::NoDestructor<HnsWalletIpfsServiceFactory> instance;
  return instance.get();
}

// static
mojo::PendingRemote<mojom::IpfsService>
HnsWalletIpfsServiceFactory::GetForContext(content::BrowserContext* context) {
  if (!IsAllowedForContext(context)) {
    return mojo::PendingRemote<mojom::IpfsService>();
  }

  return static_cast<HnsWalletIpfsService*>(
             GetInstance()->GetServiceForBrowserContext(context, true))
      ->MakeRemote();
}

// static
HnsWalletIpfsService* HnsWalletIpfsServiceFactory::GetServiceForContext(
    content::BrowserContext* context) {
  if (!IsAllowedForContext(context)) {
    return nullptr;
  }
  return static_cast<HnsWalletIpfsService*>(
      GetInstance()->GetServiceForBrowserContext(context, true));
}

// static
void HnsWalletIpfsServiceFactory::BindForContext(
    content::BrowserContext* context,
    mojo::PendingReceiver<mojom::IpfsService> receiver) {
  auto* ipfs_service =
      HnsWalletIpfsServiceFactory::GetServiceForContext(context);
  if (ipfs_service) {
    ipfs_service->Bind(std::move(receiver));
  }
}

HnsWalletIpfsServiceFactory::HnsWalletIpfsServiceFactory()
    : BrowserContextKeyedServiceFactory(
          "HnsWalletIpfsService",
          BrowserContextDependencyManager::GetInstance()) {}

HnsWalletIpfsServiceFactory::~HnsWalletIpfsServiceFactory() = default;

KeyedService* HnsWalletIpfsServiceFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  return new HnsWalletIpfsService(user_prefs::UserPrefs::Get(context));
}

content::BrowserContext* HnsWalletIpfsServiceFactory::GetBrowserContextToUse(
    content::BrowserContext* context) const {
  return chrome::GetBrowserContextRedirectedInIncognito(context);
}

}  // namespace hns_wallet

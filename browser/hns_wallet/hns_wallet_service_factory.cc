/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_wallet/hns_wallet_service_factory.h"

#include <memory>
#include <utility>

#include "base/no_destructor.h"
#include "hns/browser/hns_wallet/hns_wallet_context_utils.h"
#include "hns/browser/hns_wallet/json_rpc_service_factory.h"
#include "hns/browser/hns_wallet/keyring_service_factory.h"
#include "hns/browser/hns_wallet/tx_service_factory.h"
#include "hns/components/hns_wallet/browser/hns_wallet_service.h"
#include "hns/components/hns_wallet/browser/hns_wallet_service_delegate.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/incognito_helpers.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"
#include "components/user_prefs/user_prefs.h"
#include "content/public/browser/storage_partition.h"

namespace hns_wallet {

// static
HnsWalletServiceFactory* HnsWalletServiceFactory::GetInstance() {
  static base::NoDestructor<HnsWalletServiceFactory> instance;
  return instance.get();
}

// static
mojo::PendingRemote<mojom::HnsWalletService>
HnsWalletServiceFactory::GetForContext(content::BrowserContext* context) {
  if (!IsAllowedForContext(context)) {
    return mojo::PendingRemote<mojom::HnsWalletService>();
  }

  return static_cast<HnsWalletService*>(
             GetInstance()->GetServiceForBrowserContext(context, true))
      ->MakeRemote();
}

// static
HnsWalletService* HnsWalletServiceFactory::GetServiceForContext(
    content::BrowserContext* context) {
  if (!IsAllowedForContext(context)) {
    return nullptr;
  }
  return static_cast<HnsWalletService*>(
      GetInstance()->GetServiceForBrowserContext(context, true));
}

// static
void HnsWalletServiceFactory::BindForContext(
    content::BrowserContext* context,
    mojo::PendingReceiver<mojom::HnsWalletService> receiver) {
  auto* hns_wallet_service =
      HnsWalletServiceFactory::GetServiceForContext(context);
  if (hns_wallet_service) {
    hns_wallet_service->Bind(std::move(receiver));
  }
}

HnsWalletServiceFactory::HnsWalletServiceFactory()
    : BrowserContextKeyedServiceFactory(
          "HnsWalletService",
          BrowserContextDependencyManager::GetInstance()) {
  DependsOn(KeyringServiceFactory::GetInstance());
  DependsOn(JsonRpcServiceFactory::GetInstance());
  DependsOn(TxServiceFactory::GetInstance());
}

HnsWalletServiceFactory::~HnsWalletServiceFactory() = default;

KeyedService* HnsWalletServiceFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  auto* default_storage_partition = context->GetDefaultStoragePartition();
  auto shared_url_loader_factory =
      default_storage_partition->GetURLLoaderFactoryForBrowserProcess();
  return new HnsWalletService(
      shared_url_loader_factory, HnsWalletServiceDelegate::Create(context),
      KeyringServiceFactory::GetServiceForContext(context),
      JsonRpcServiceFactory::GetServiceForContext(context),
      TxServiceFactory::GetServiceForContext(context),
      user_prefs::UserPrefs::Get(context), g_browser_process->local_state());
}

content::BrowserContext* HnsWalletServiceFactory::GetBrowserContextToUse(
    content::BrowserContext* context) const {
  return chrome::GetBrowserContextRedirectedInIncognito(context);
}

}  // namespace hns_wallet

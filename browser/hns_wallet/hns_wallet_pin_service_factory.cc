// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "hns/browser/hns_wallet/hns_wallet_pin_service_factory.h"

#include <memory>
#include <utility>

#include "base/no_destructor.h"
#include "hns/browser/hns_wallet/hns_wallet_context_utils.h"
#include "hns/browser/hns_wallet/json_rpc_service_factory.h"
// TODO(cypt4) : Refactor hns/browser into separate component (#27486)
#include "hns/browser/ipfs/ipfs_local_pin_service_factory.h"  // nogncheck
#include "hns/browser/ipfs/ipfs_service_factory.h"            // nogncheck
#include "hns/components/hns_wallet/browser/hns_wallet_pin_service.h"
#include "hns/components/hns_wallet/browser/hns_wallet_service.h"
#include "hns/components/hns_wallet/browser/hns_wallet_service_delegate.h"
#include "hns/components/hns_wallet/common/common_utils.h"
#include "chrome/browser/profiles/incognito_helpers.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"
#include "components/user_prefs/user_prefs.h"
#include "content/public/browser/storage_partition.h"

namespace hns_wallet {

// static
HnsWalletPinServiceFactory* HnsWalletPinServiceFactory::GetInstance() {
  static base::NoDestructor<HnsWalletPinServiceFactory> instance;
  return instance.get();
}

// static
mojo::PendingRemote<mojom::WalletPinService>
HnsWalletPinServiceFactory::GetForContext(content::BrowserContext* context) {
  if (!IsAllowedForContext(context)) {
    return mojo::PendingRemote<mojom::WalletPinService>();
  }

  auto* service = GetServiceForContext(context);
  if (!service) {
    return mojo::PendingRemote<mojom::WalletPinService>();
  }

  return service->MakeRemote();
}

// static
HnsWalletPinService* HnsWalletPinServiceFactory::GetServiceForContext(
    content::BrowserContext* context) {
  if (!IsAllowedForContext(context)) {
    return nullptr;
  }
  if (!ipfs::IpfsServiceFactory::IsIpfsEnabled(context)) {
    return nullptr;
  }
  if (!hns_wallet::IsNftPinningEnabled()) {
    return nullptr;
  }
  return static_cast<HnsWalletPinService*>(
      GetInstance()->GetServiceForBrowserContext(context, true));
}

// static
void HnsWalletPinServiceFactory::BindForContext(
    content::BrowserContext* context,
    mojo::PendingReceiver<mojom::WalletPinService> receiver) {
  auto* service = HnsWalletPinServiceFactory::GetServiceForContext(context);
  if (service) {
    service->Bind(std::move(receiver));
  }
}

HnsWalletPinServiceFactory::HnsWalletPinServiceFactory()
    : BrowserContextKeyedServiceFactory(
          "HnsWalletPinService",
          BrowserContextDependencyManager::GetInstance()) {
  DependsOn(hns_wallet::JsonRpcServiceFactory::GetInstance());
  DependsOn(ipfs::IpfsLocalPinServiceFactory::GetInstance());
  DependsOn(ipfs::IpfsServiceFactory::GetInstance());
}

HnsWalletPinServiceFactory::~HnsWalletPinServiceFactory() = default;

KeyedService* HnsWalletPinServiceFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  return new HnsWalletPinService(
      user_prefs::UserPrefs::Get(context),
      JsonRpcServiceFactory::GetServiceForContext(context),
      ipfs::IpfsLocalPinServiceFactory::GetServiceForContext(context),
      ipfs::IpfsServiceFactory::GetForContext(context),
      std::make_unique<ContentTypeChecker>(
          user_prefs::UserPrefs::Get(context),
          context->GetDefaultStoragePartition()
              ->GetURLLoaderFactoryForBrowserProcess()));
}

content::BrowserContext* HnsWalletPinServiceFactory::GetBrowserContextToUse(
    content::BrowserContext* context) const {
  return chrome::GetBrowserContextRedirectedInIncognito(context);
}

}  // namespace hns_wallet

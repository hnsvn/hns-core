// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "hns/browser/hns_wallet/hns_wallet_auto_pin_service_factory.h"

#include <memory>
#include <utility>

#include "base/no_destructor.h"
#include "hns/browser/hns_wallet/hns_wallet_context_utils.h"
#include "hns/browser/hns_wallet/hns_wallet_pin_service_factory.h"
#include "hns/browser/hns_wallet/hns_wallet_service_factory.h"
// TODO(cypt4) : Refactor hns/browser/ipfs into separate component (#27486)
#include "hns/browser/ipfs/ipfs_service_factory.h"  // nogncheck

#include "hns/components/hns_wallet/browser/hns_wallet_pin_service.h"
#include "hns/components/hns_wallet/browser/hns_wallet_service.h"
#include "hns/components/hns_wallet/common/common_utils.h"

#include "chrome/browser/profiles/incognito_helpers.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"
#include "components/user_prefs/user_prefs.h"

namespace hns_wallet {

// static
HnsWalletAutoPinServiceFactory*
HnsWalletAutoPinServiceFactory::GetInstance() {
  static base::NoDestructor<HnsWalletAutoPinServiceFactory> instance;
  return instance.get();
}

// static
mojo::PendingRemote<mojom::WalletAutoPinService>
HnsWalletAutoPinServiceFactory::GetForContext(
    content::BrowserContext* context) {
  if (!IsAllowedForContext(context)) {
    return mojo::PendingRemote<mojom::WalletAutoPinService>();
  }

  auto* service = GetServiceForContext(context);

  if (!service) {
    return mojo::PendingRemote<mojom::WalletAutoPinService>();
  }

  return service->MakeRemote();
}

// static
HnsWalletAutoPinService*
HnsWalletAutoPinServiceFactory::GetServiceForContext(
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
  return static_cast<HnsWalletAutoPinService*>(
      GetInstance()->GetServiceForBrowserContext(context, true));
}

// static
void HnsWalletAutoPinServiceFactory::BindForContext(
    content::BrowserContext* context,
    mojo::PendingReceiver<mojom::WalletAutoPinService> receiver) {
  auto* service =
      HnsWalletAutoPinServiceFactory::GetServiceForContext(context);
  if (service) {
    service->Bind(std::move(receiver));
  }
}

HnsWalletAutoPinServiceFactory::HnsWalletAutoPinServiceFactory()
    : BrowserContextKeyedServiceFactory(
          "HnsWalletAutoPinService",
          BrowserContextDependencyManager::GetInstance()) {
  DependsOn(HnsWalletServiceFactory::GetInstance());
  DependsOn(HnsWalletPinServiceFactory::GetInstance());
}

HnsWalletAutoPinServiceFactory::~HnsWalletAutoPinServiceFactory() = default;

KeyedService* HnsWalletAutoPinServiceFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  return new HnsWalletAutoPinService(
      user_prefs::UserPrefs::Get(context),
      HnsWalletServiceFactory::GetServiceForContext(context),
      HnsWalletPinServiceFactory::GetServiceForContext(context));
}

content::BrowserContext*
HnsWalletAutoPinServiceFactory::GetBrowserContextToUse(
    content::BrowserContext* context) const {
  return chrome::GetBrowserContextRedirectedInIncognito(context);
}

}  // namespace hns_wallet

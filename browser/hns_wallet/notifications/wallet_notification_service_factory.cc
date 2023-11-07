/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_wallet/notifications/wallet_notification_service_factory.h"

#include <memory>

#include "base/no_destructor.h"
#include "hns/browser/hns_wallet/hns_wallet_context_utils.h"
#include "hns/browser/hns_wallet/notifications/wallet_notification_service.h"
#include "hns/browser/hns_wallet/tx_service_factory.h"
#include "chrome/browser/notifications/notification_display_service_factory.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"

namespace hns_wallet {

// static
WalletNotificationServiceFactory*
WalletNotificationServiceFactory::GetInstance() {
  static base::NoDestructor<WalletNotificationServiceFactory> instance;
  return instance.get();
}

WalletNotificationServiceFactory::WalletNotificationServiceFactory()
    : BrowserContextKeyedServiceFactory(
          "WalletNotificationService",
          BrowserContextDependencyManager::GetInstance()) {
  DependsOn(NotificationDisplayServiceFactory::GetInstance());
  DependsOn(hns_wallet::TxServiceFactory::GetInstance());
}

WalletNotificationServiceFactory::~WalletNotificationServiceFactory() = default;

KeyedService* WalletNotificationServiceFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  return new WalletNotificationService(context);
}

// static
WalletNotificationService*
WalletNotificationServiceFactory::GetServiceForContext(
    content::BrowserContext* context) {
  if (!IsAllowedForContext(context)) {
    return nullptr;
  }
  return static_cast<WalletNotificationService*>(
      GetInstance()->GetServiceForBrowserContext(context, true));
}

}  // namespace hns_wallet

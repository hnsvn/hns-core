/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_WALLET_NOTIFICATIONS_WALLET_NOTIFICATION_SERVICE_FACTORY_H_
#define HNS_BROWSER_HNS_WALLET_NOTIFICATIONS_WALLET_NOTIFICATION_SERVICE_FACTORY_H_

#include "hns/browser/hns_wallet/notifications/wallet_notification_service.h"
#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

namespace hns_wallet {

// Singleton that owns all WalletNotificationService and associates them with
// BrowserContext.
class WalletNotificationServiceFactory
    : public BrowserContextKeyedServiceFactory {
 public:
  WalletNotificationServiceFactory(const WalletNotificationServiceFactory&) =
      delete;
  WalletNotificationServiceFactory& operator=(
      const WalletNotificationServiceFactory&) = delete;

  static WalletNotificationServiceFactory* GetInstance();
  static WalletNotificationService* GetServiceForContext(
      content::BrowserContext* context);

 private:
  friend base::NoDestructor<WalletNotificationServiceFactory>;

  WalletNotificationServiceFactory();
  ~WalletNotificationServiceFactory() override;

  // BrowserContextKeyedServiceFactory:
  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const override;
};

}  // namespace hns_wallet

#endif  // HNS_BROWSER_HNS_WALLET_NOTIFICATIONS_WALLET_NOTIFICATION_SERVICE_FACTORY_H_

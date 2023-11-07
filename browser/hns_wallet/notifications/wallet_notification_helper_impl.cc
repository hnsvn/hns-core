/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_wallet/wallet_notification_helper.h"

#include "hns/browser/hns_wallet/notifications/wallet_notification_service_factory.h"
#include "hns/components/hns_wallet/browser/tx_service.h"

namespace hns_wallet {

void RegisterWalletNotificationService(content::BrowserContext* context,
                                       TxService* tx_service) {
  auto* notification_service =
      WalletNotificationServiceFactory::GetInstance()->GetServiceForContext(
          context);
  tx_service->AddObserver(notification_service->GetReceiver());
}

}  // namespace hns_wallet

/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_WALLET_NOTIFICATIONS_WALLET_NOTIFICATION_SERVICE_H_
#define HNS_BROWSER_HNS_WALLET_NOTIFICATIONS_WALLET_NOTIFICATION_SERVICE_H_

#include <string>

#include "base/memory/raw_ptr.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "components/keyed_service/core/keyed_service.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/receiver.h"

namespace content {
class BrowserContext;
}  // namespace content

namespace hns_wallet {

class WalletNotificationService : public KeyedService,
                                  public mojom::TxServiceObserver {
 public:
  explicit WalletNotificationService(content::BrowserContext* context);
  ~WalletNotificationService() override;
  WalletNotificationService(const WalletNotificationService&) = delete;
  WalletNotificationService operator=(const WalletNotificationService&) =
      delete;

  // mojom::TxServiceObserver
  void OnNewUnapprovedTx(mojom::TransactionInfoPtr tx_info) override {}
  void OnUnapprovedTxUpdated(mojom::TransactionInfoPtr tx_info) override {}
  void OnTransactionStatusChanged(mojom::TransactionInfoPtr tx_info) override;
  void OnTxServiceReset() override {}

  mojo::PendingRemote<mojom::TxServiceObserver> GetReceiver() {
    return tx_observer_receiver_.BindNewPipeAndPassRemote();
  }

 private:
  friend class WalletNotificationServiceUnitTest;

  bool ShouldDisplayUserNotification(mojom::TransactionStatus status);
  void DisplayUserNotification(mojom::TransactionStatus status,
                               const std::string& address,
                               const std::string& tx_id);

  raw_ptr<content::BrowserContext> context_;
  mojo::Receiver<mojom::TxServiceObserver> tx_observer_receiver_{this};
};

}  // namespace hns_wallet

#endif  // HNS_BROWSER_HNS_WALLET_NOTIFICATIONS_WALLET_NOTIFICATION_SERVICE_H_

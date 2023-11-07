/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_TOOLBAR_WALLET_BUTTON_NOTIFICATION_SOURCE_H_
#define HNS_BROWSER_UI_VIEWS_TOOLBAR_WALLET_BUTTON_NOTIFICATION_SOURCE_H_

#include "base/memory/weak_ptr.h"
#include "hns/components/hns_wallet/browser/keyring_service.h"
#include "hns/components/hns_wallet/browser/keyring_service_observer_base.h"
#include "hns/components/hns_wallet/browser/tx_service.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "chrome/browser/profiles/profile.h"
#include "components/prefs/pref_service.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "mojo/public/cpp/bindings/remote.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns {

using WalletButtonNotificationSourceCallback =
    base::RepeatingCallback<void(bool /* show suggest */,
                                 size_t /* counter */)>;

// Provides and updates data for the wallet button notification badge.
// Like number of pending transactions or onboarding bubble to show.
class WalletButtonNotificationSource
    : hns_wallet::mojom::TxServiceObserver,
      hns_wallet::KeyringServiceObserverBase {
 public:
  WalletButtonNotificationSource(
      Profile* profile,
      WalletButtonNotificationSourceCallback callback);
  ~WalletButtonNotificationSource() override;

  void MarkWalletButtonWasClicked();
  void Init();

 private:
  void EnsureTxServiceConnected();
  void OnTxServiceConnectionError();

  void EnsureKeyringServiceConnected();
  void OnKeyringServiceConnectionError();

  // hns_wallet::mojom::TxServiceObserver
  void OnNewUnapprovedTx(
      hns_wallet::mojom::TransactionInfoPtr tx_info) override;
  void OnUnapprovedTxUpdated(
      hns_wallet::mojom::TransactionInfoPtr tx_info) override {}
  void OnTransactionStatusChanged(
      hns_wallet::mojom::TransactionInfoPtr tx_info) override;
  void OnTxServiceReset() override;

  // hns_wallet::KeyringServiceObserverBase
  void KeyringCreated(hns_wallet::mojom::KeyringId keyring_id) override;
  void KeyringRestored(hns_wallet::mojom::KeyringId keyring_id) override;

  void OnKeyringReady(hns_wallet::mojom::KeyringId keyring_id);
  void CheckTxStatus();
  void OnTxStatusResolved(uint32_t count);
  void OnKeyringInfoResolved(hns_wallet::mojom::KeyringInfoPtr keyring_info);

  void NotifyObservers();

  raw_ptr<Profile> profile_ = nullptr;
  raw_ptr<PrefService> prefs_ = nullptr;
  mojo::Remote<hns_wallet::mojom::TxService> tx_service_;
  mojo::Remote<hns_wallet::mojom::KeyringService> keyring_service_;

  mojo::Receiver<hns_wallet::mojom::TxServiceObserver> tx_observer_{this};
  mojo::Receiver<hns_wallet::mojom::KeyringServiceObserver>
      keyring_service_observer_{this};

  WalletButtonNotificationSourceCallback callback_;

  absl::optional<bool> wallet_created_;
  uint32_t pending_tx_count_ = 0;

  base::WeakPtrFactory<WalletButtonNotificationSource> weak_ptr_factory_{this};
};

}  // namespace hns

#endif  // HNS_BROWSER_UI_VIEWS_TOOLBAR_WALLET_BUTTON_NOTIFICATION_SOURCE_H_

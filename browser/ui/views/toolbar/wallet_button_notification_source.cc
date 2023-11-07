/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/toolbar/wallet_button_notification_source.h"

#include <utility>

#include "hns/browser/hns_wallet/keyring_service_factory.h"
#include "hns/browser/hns_wallet/tx_service_factory.h"
#include "hns/components/hns_wallet/browser/pref_names.h"

namespace hns {

namespace {
constexpr base::TimeDelta kReconnectTime = base::Seconds(5);
}  // namespace

WalletButtonNotificationSource::WalletButtonNotificationSource(
    Profile* profile,
    WalletButtonNotificationSourceCallback callback)
    : profile_(profile), callback_(callback) {
  prefs_ = profile->GetPrefs();
}

void WalletButtonNotificationSource::Init() {
  EnsureKeyringServiceConnected();
  EnsureTxServiceConnected();
}

void WalletButtonNotificationSource::EnsureTxServiceConnected() {
  if (tx_service_) {
    return;
  }
  auto pending = hns_wallet::TxServiceFactory::GetForContext(profile_);
  tx_service_.Bind(std::move(pending));
  tx_service_.set_disconnect_handler(base::BindOnce(
      &WalletButtonNotificationSource::OnTxServiceConnectionError,
      weak_ptr_factory_.GetWeakPtr()));
  tx_service_->AddObserver(tx_observer_.BindNewPipeAndPassRemote());
  CheckTxStatus();
}

void WalletButtonNotificationSource::EnsureKeyringServiceConnected() {
  if (!keyring_service_) {
    auto pending = hns_wallet::KeyringServiceFactory::GetForContext(profile_);
    keyring_service_.Bind(std::move(pending));
    keyring_service_.set_disconnect_handler(base::BindOnce(
        &WalletButtonNotificationSource::OnTxServiceConnectionError,
        weak_ptr_factory_.GetWeakPtr()));
    keyring_service_->AddObserver(
        keyring_service_observer_.BindNewPipeAndPassRemote());
    keyring_service_->GetKeyringInfo(
        hns_wallet::mojom::kDefaultKeyringId,
        base::BindOnce(&WalletButtonNotificationSource::OnKeyringInfoResolved,
                       weak_ptr_factory_.GetWeakPtr()));
  }
}

void WalletButtonNotificationSource::OnTxServiceConnectionError() {
  tx_service_.reset();
  tx_observer_.reset();
  base::SequencedTaskRunner::GetCurrentDefault()->PostDelayedTask(
      FROM_HERE,
      base::BindOnce(&WalletButtonNotificationSource::EnsureTxServiceConnected,
                     weak_ptr_factory_.GetWeakPtr()),
      kReconnectTime);
}

void WalletButtonNotificationSource::OnKeyringServiceConnectionError() {
  keyring_service_.reset();
  keyring_service_observer_.reset();
  base::SequencedTaskRunner::GetCurrentDefault()->PostDelayedTask(
      FROM_HERE,
      base::BindOnce(
          &WalletButtonNotificationSource::EnsureKeyringServiceConnected,
          weak_ptr_factory_.GetWeakPtr()),
      kReconnectTime);
}

WalletButtonNotificationSource::~WalletButtonNotificationSource() = default;

void WalletButtonNotificationSource::MarkWalletButtonWasClicked() {
  prefs_->SetBoolean(kShouldShowWalletSuggestionBadge, false);
  NotifyObservers();
}

void WalletButtonNotificationSource::CheckTxStatus() {
  if (!tx_service_) {
    return;
  }
  tx_service_->GetPendingTransactionsCount(
      base::BindOnce(&WalletButtonNotificationSource::OnTxStatusResolved,
                     weak_ptr_factory_.GetWeakPtr()));
}

void WalletButtonNotificationSource::OnTransactionStatusChanged(
    hns_wallet::mojom::TransactionInfoPtr tx_info) {
  CheckTxStatus();
}

void WalletButtonNotificationSource::OnNewUnapprovedTx(
    hns_wallet::mojom::TransactionInfoPtr tx_info) {
  CheckTxStatus();
}

void WalletButtonNotificationSource::OnTxServiceReset() {
  OnTxStatusResolved(0u);
}

void WalletButtonNotificationSource::OnTxStatusResolved(uint32_t count) {
  pending_tx_count_ = count;
  NotifyObservers();
}

void WalletButtonNotificationSource::OnKeyringReady(
    hns_wallet::mojom::KeyringId keyring_id) {
  if (keyring_id == hns_wallet::mojom::kDefaultKeyringId) {
    prefs_->SetBoolean(kShouldShowWalletSuggestionBadge, false);
    NotifyObservers();
  }
}

void WalletButtonNotificationSource::KeyringCreated(
    hns_wallet::mojom::KeyringId keyring_id) {
  OnKeyringReady(keyring_id);
}

void WalletButtonNotificationSource::KeyringRestored(
    hns_wallet::mojom::KeyringId keyring_id) {
  OnKeyringReady(keyring_id);
}

void WalletButtonNotificationSource::NotifyObservers() {
  bool show_suggestion_badge =
      (wallet_created_.has_value() && !wallet_created_.value() &&
       prefs_->GetBoolean(kShouldShowWalletSuggestionBadge));
  callback_.Run(show_suggestion_badge, pending_tx_count_);
}

void WalletButtonNotificationSource::OnKeyringInfoResolved(
    hns_wallet::mojom::KeyringInfoPtr keyring_info) {
  wallet_created_ = keyring_info && keyring_info->is_keyring_created;
  if (wallet_created_.value()) {
    prefs_->SetBoolean(kShouldShowWalletSuggestionBadge, false);
  }
  NotifyObservers();
}

}  // namespace hns

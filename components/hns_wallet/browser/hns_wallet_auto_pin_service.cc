// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "hns/components/hns_wallet/browser/hns_wallet_auto_pin_service.h"

#include "hns/components/hns_wallet/browser/pref_names.h"

namespace hns_wallet {
namespace {
bool ShouldRetryOnError(const mojom::PinErrorPtr& error) {
  return !error || error->error_code !=
                       mojom::WalletPinServiceErrorCode::ERR_NON_IPFS_TOKEN_URL;
}

absl::optional<std::string> GetTokenStringValue(
    const mojom::BlockchainTokenPtr& token) {
  return HnsWalletPinService::GetTokenPrefPath(absl::nullopt, token);
}

}  // namespace

HnsWalletAutoPinService::IntentData::IntentData(
    const BlockchainTokenPtr& token,
    Operation operation,
    absl::optional<std::string> service)
    : token(token.Clone()), operation(operation), service(std::move(service)) {}

HnsWalletAutoPinService::IntentData::~IntentData() = default;

bool HnsWalletAutoPinService::IntentData::Equals(
    const std::unique_ptr<HnsWalletAutoPinService::IntentData>& other) {
  if (!other) {
    return false;
  }
  return (this->operation == other->operation &&
          HnsWalletPinService::GetTokenPrefPath(this->service, this->token) ==
              HnsWalletPinService::GetTokenPrefPath(other->service,
                                                      other->token));
}

HnsWalletAutoPinService::HnsWalletAutoPinService(
    PrefService* prefs,
    HnsWalletService* hns_wallet_service,
    HnsWalletPinService* hns_wallet_pin_service)
    : pref_service_(prefs),
      hns_wallet_service_(hns_wallet_service),
      hns_wallet_pin_service_(hns_wallet_pin_service) {
  DCHECK(hns_wallet_service);
  hns_wallet_service->AddTokenObserver(
      token_observer_.BindNewPipeAndPassRemote());
  if (IsAutoPinEnabled()) {
    Restore();
  }

  pref_change_registrar_ = std::make_unique<PrefChangeRegistrar>();
  pref_change_registrar_->Init(pref_service_);
  pref_change_registrar_->Add(
      kAutoPinEnabled,
      base::BindRepeating(&HnsWalletAutoPinService::OnAutoPinStatusChanged,
                          weak_ptr_factory_.GetWeakPtr()));

  hns_wallet_service->AddObserver(
      hns_wallet_service_observer_.BindNewPipeAndPassRemote());
}

void HnsWalletAutoPinService::OnResetWallet() {
  Reset();
}

void HnsWalletAutoPinService::ResetLocalState() {
  tasks_weak_ptr_factory_.InvalidateWeakPtrs();
  tokens_.clear();
  queue_.clear();
  current_.reset();
}

void HnsWalletAutoPinService::Reset() {
  ResetLocalState();
  SetAutoPinEnabled(false);
  hns_wallet_pin_service_->Reset(base::DoNothing());
}

void HnsWalletAutoPinService::OnAutoPinStatusChanged() {
  auto enabled = IsAutoPinEnabled();
  if (enabled) {
    Restore();
  } else {
    ResetLocalState();
  }
  for (const auto& observer : observers_) {
    observer->OnAutoPinStatusChanged(enabled);
  }
}

HnsWalletAutoPinService::~HnsWalletAutoPinService() {}

void HnsWalletAutoPinService::Bind(
    mojo::PendingReceiver<mojom::WalletAutoPinService> receiver) {
  receivers_.Add(this, std::move(receiver));
}

mojo::PendingRemote<mojom::WalletAutoPinService>
HnsWalletAutoPinService::MakeRemote() {
  mojo::PendingRemote<WalletAutoPinService> remote;
  receivers_.Add(this, remote.InitWithNewPipeAndPassReceiver());
  return remote;
}

void HnsWalletAutoPinService::OnTokenAdded(BlockchainTokenPtr token) {
  if (!IsAutoPinEnabled()) {
    return;
  }
  if (!HnsWalletPinService::IsTokenSupportedForPinning(token)) {
    return;
  }
  auto token_str = GetTokenStringValue(token);
  if (!token_str) {
    return;
  }
  tokens_.insert(token_str.value());
  base::EraseIf(queue_, [&token_str](const auto& intent) {
    return GetTokenStringValue(intent->token) == token_str;
  });
  AddOrExecute(
      std::make_unique<IntentData>(token, Operation::kAdd, absl::nullopt));
}

void HnsWalletAutoPinService::OnTokenRemoved(BlockchainTokenPtr token) {
  if (!IsAutoPinEnabled()) {
    return;
  }
  if (!HnsWalletPinService::IsTokenSupportedForPinning(token)) {
    return;
  }
  auto token_str = GetTokenStringValue(token);
  if (!token_str) {
    return;
  }
  tokens_.erase(token_str.value());
  base::EraseIf(queue_, [&token_str](const auto& intent) {
    return GetTokenStringValue(intent->token) == token_str;
  });
  AddOrExecute(
      std::make_unique<IntentData>(token, Operation::kDelete, absl::nullopt));
}

void HnsWalletAutoPinService::Restore() {
  if (!IsAutoPinEnabled()) {
    NOTREACHED();
    return;
  }
  tokens_.clear();
  hns_wallet_pin_service_->Restore();
  hns_wallet_service_->GetAllUserAssets(
      base::BindOnce(&HnsWalletAutoPinService::OnTokenListResolved,
                     tasks_weak_ptr_factory_.GetWeakPtr()));
}

void HnsWalletAutoPinService::OnTokenListResolved(
    std::vector<BlockchainTokenPtr> token_list) {
  if (!IsAutoPinEnabled()) {
    return;
  }
  // Resolves list of user tokens.
  // Check whether they are pinned or not and posts corresponding tasks.
  std::set<std::string> known_tokens =
      hns_wallet_pin_service_->GetTokens(absl::nullopt);
  for (const auto& token : token_list) {
    if (!HnsWalletPinService::IsTokenSupportedForPinning(token)) {
      continue;
    }

    auto token_path =
        HnsWalletPinService::GetTokenPrefPath(absl::nullopt, token);
    auto token_str = GetTokenStringValue(token);

    // Currently they are same but may be different in the future
    if (!token_path || !token_str) {
      continue;
    }

    known_tokens.erase(token_path.value());
    tokens_.insert(token_str.value());

    mojom::TokenPinStatusPtr status =
        hns_wallet_pin_service_->GetTokenStatus(absl::nullopt, token);

    std::unique_ptr<IntentData> intent;
    if (!status) {
      AddOrExecute(
          std::make_unique<IntentData>(token, Operation::kAdd, absl::nullopt));
    } else if (status->code == mojom::TokenPinStatusCode::STATUS_PINNED) {
      // Pinned tokens should be verified for entirety time to time.
      // We should check that related CIDs are still pinned.
      auto t1 = status->validate_time;
      if ((base::Time::Now() - t1) > base::Days(1) || t1 > base::Time::Now()) {
        AddOrExecute(std::make_unique<IntentData>(token, Operation::kValidate,
                                                  absl::nullopt));
      }
    } else if (status->code ==
               mojom::TokenPinStatusCode::STATUS_PINNING_FAILED) {
      if (ShouldRetryOnError(status->error)) {
        AddOrExecute(std::make_unique<IntentData>(token, Operation::kAdd,
                                                  absl::nullopt));
      }
    } else {
      AddOrExecute(
          std::make_unique<IntentData>(token, Operation::kAdd, absl::nullopt));
    }
  }

  // Tokens that were previously pinned but not listed in the wallet should be
  // unpinned.
  for (const auto& t : known_tokens) {
    mojom::BlockchainTokenPtr token =
        HnsWalletPinService::TokenFromPrefPath(t);
    if (token) {
      AddOrExecute(std::make_unique<IntentData>(token, Operation::kDelete,
                                                absl::nullopt));
    }
  }

  CheckQueue();
}

void HnsWalletAutoPinService::ValidateToken(
    const std::unique_ptr<IntentData>& data) {
  hns_wallet_pin_service_->Validate(
      data->token->Clone(), data->service,
      base::BindOnce(&HnsWalletAutoPinService::OnValidateTaskFinished,
                     tasks_weak_ptr_factory_.GetWeakPtr()));
}

void HnsWalletAutoPinService::PinToken(
    const std::unique_ptr<IntentData>& data) {
  hns_wallet_pin_service_->AddPin(
      data->token->Clone(), data->service,
      base::BindOnce(&HnsWalletAutoPinService::OnTaskFinished,
                     tasks_weak_ptr_factory_.GetWeakPtr()));
}

void HnsWalletAutoPinService::UnpinToken(
    const std::unique_ptr<IntentData>& data) {
  hns_wallet_pin_service_->RemovePin(
      data->token->Clone(), data->service,
      base::BindOnce(&HnsWalletAutoPinService::OnTaskFinished,
                     tasks_weak_ptr_factory_.GetWeakPtr()));
}

void HnsWalletAutoPinService::AddOrExecute(std::unique_ptr<IntentData> data) {
  if (!IsAutoPinEnabled()) {
    return;
  }
  DCHECK(data);
  for (const auto& v : queue_) {
    if (v->Equals(data)) {
      return;
    }
  }
  if (current_ && current_->Equals(data)) {
    return;
  }

  auto token_str = GetTokenStringValue(data->token);
  if (!token_str) {
    return;
  }

  if (data->operation == Operation::kAdd ||
      data->operation == Operation::kValidate) {
    if (!base::Contains(tokens_, token_str.value())) {
      return;
    }
  }
  if (data->operation == Operation::kDelete) {
    if (base::Contains(tokens_, token_str.value())) {
      return;
    }
  }

  if (data->operation == Operation::kAdd) {
    hns_wallet_pin_service_->MarkAsPendingForPinning(data->token,
                                                       data->service);
  } else if (data->operation == Operation::kDelete) {
    hns_wallet_pin_service_->MarkAsPendingForUnpinning(data->token,
                                                         data->service);
  }
  queue_.push_back(std::move(data));
  CheckQueue();
}

void HnsWalletAutoPinService::PostRetry(std::unique_ptr<IntentData> data) {
  if (!IsAutoPinEnabled()) {
    return;
  }
  int multiply = ++data->attempt;
  base::SequencedTaskRunner::GetCurrentDefault()->PostDelayedTask(
      FROM_HERE,
      base::BindOnce(&HnsWalletAutoPinService::AddOrExecute,
                     tasks_weak_ptr_factory_.GetWeakPtr(), std::move(data)),
      base::Minutes(2 * multiply));
}

void HnsWalletAutoPinService::CheckQueue() {
  if (!IsAutoPinEnabled()) {
    return;
  }
  if (queue_.empty() || current_) {
    return;
  }

  current_ = std::move(queue_.front());
  queue_.pop_front();

  if (current_->operation == Operation::kAdd) {
    PinToken(current_);
  } else if (current_->operation == Operation::kDelete) {
    UnpinToken(current_);
  } else if (current_->operation == Operation::kValidate) {
    ValidateToken(current_);
  }
}

void HnsWalletAutoPinService::OnTaskFinished(bool result,
                                               mojom::PinErrorPtr error) {
  if (!result &&
      (current_->operation != Operation::kAdd || ShouldRetryOnError(error))) {
    PostRetry(std::move(current_));
  }
  current_.reset();
  CheckQueue();
}

void HnsWalletAutoPinService::OnValidateTaskFinished(
    mojom::TokenValidationResult result) {
  if (result == mojom::TokenValidationResult::kValidationError) {
    PostRetry(std::move(current_));
  }
  auto current = std::move(current_);
  if (result == mojom::TokenValidationResult::kValidationFailed) {
    AddOrExecute(std::make_unique<IntentData>(current->token, Operation::kAdd,
                                              current->service));
  }
  CheckQueue();
}

void HnsWalletAutoPinService::SetAutoPinEnabled(bool enabled) {
  pref_service_->SetBoolean(kAutoPinEnabled, enabled);
}

bool HnsWalletAutoPinService::IsAutoPinEnabled() {
  return pref_service_->GetBoolean(kAutoPinEnabled);
}

void HnsWalletAutoPinService::IsAutoPinEnabled(
    IsAutoPinEnabledCallback callback) {
  std::move(callback).Run(IsAutoPinEnabled());
}

void HnsWalletAutoPinService::AddObserver(
    ::mojo::PendingRemote<mojom::WalletAutoPinServiceObserver> observer) {
  observers_.Add(std::move(observer));
}

}  // namespace hns_wallet

/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_CONFIRMATIONS_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_CONFIRMATIONS_H_

#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "hns/components/hns_ads/core/internal/account/confirmations/confirmations_delegate.h"
#include "hns/components/hns_ads/core/internal/account/confirmations/queue/confirmation_queue.h"
#include "hns/components/hns_ads/core/internal/account/confirmations/queue/confirmation_queue_delegate.h"
#include "hns/components/hns_ads/core/internal/account/utility/redeem_confirmation/redeem_confirmation_delegate.h"

namespace hns_ads {

class TokenGeneratorInterface;
struct TransactionInfo;
struct UserDataInfo;

class Confirmations final : public ConfirmationQueueDelegate,
                            public RedeemConfirmationDelegate {
 public:
  explicit Confirmations(TokenGeneratorInterface* token_generator);

  Confirmations(const Confirmations&) = delete;
  Confirmations& operator=(const Confirmations&) = delete;

  Confirmations(Confirmations&&) noexcept = delete;
  Confirmations& operator=(Confirmations&&) noexcept = delete;

  ~Confirmations() override;

  void SetDelegate(ConfirmationDelegate* delegate) {
    CHECK_EQ(delegate_, nullptr);
    delegate_ = delegate;
  }

  void Confirm(const TransactionInfo& transaction);

 private:
  void ConfirmCallback(const TransactionInfo& transaction,
                       const UserDataInfo& user_data);

  // ConfirmationQueueDelegate:
  void OnDidAddConfirmationToQueue(
      const ConfirmationInfo& confirmation) override;
  void OnWillProcessConfirmationQueue(const ConfirmationInfo& confirmation,
                                      base::Time process_at) override;
  void OnDidProcessConfirmationQueue(
      const ConfirmationInfo& confirmation) override;
  void OnFailedToProcessConfirmationQueue(
      const ConfirmationInfo& confirmation) override;
  void OnDidExhaustConfirmationQueue() override;

  raw_ptr<ConfirmationDelegate> delegate_ = nullptr;

  const raw_ptr<TokenGeneratorInterface> token_generator_ =
      nullptr;  // NOT OWNED

  ConfirmationQueue queue_;

  base::WeakPtrFactory<Confirmations> weak_factory_{this};
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_CONFIRMATIONS_H_

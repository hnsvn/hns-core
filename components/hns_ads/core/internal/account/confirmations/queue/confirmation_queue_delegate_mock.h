/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_QUEUE_CONFIRMATION_QUEUE_DELEGATE_MOCK_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_QUEUE_CONFIRMATION_QUEUE_DELEGATE_MOCK_H_

#include "hns/components/hns_ads/core/internal/account/confirmations/queue/confirmation_queue_delegate.h"

#include "testing/gmock/include/gmock/gmock.h"

namespace hns_ads {

struct ConfirmationInfo;

class ConfirmationQueueDelegateMock : public ConfirmationQueueDelegate {
 public:
  ConfirmationQueueDelegateMock();

  ConfirmationQueueDelegateMock(const ConfirmationQueueDelegateMock&) = delete;
  ConfirmationQueueDelegateMock& operator=(
      const ConfirmationQueueDelegateMock&) = delete;

  ConfirmationQueueDelegateMock(ConfirmationQueueDelegateMock&&) noexcept =
      delete;
  ConfirmationQueueDelegateMock& operator=(
      ConfirmationQueueDelegateMock&&) noexcept = delete;

  ~ConfirmationQueueDelegateMock() override;

  MOCK_METHOD(void,
              OnDidAddConfirmationToQueue,
              (const ConfirmationInfo& confirmation));

  MOCK_METHOD(void,
              OnWillProcessConfirmationQueue,
              (const ConfirmationInfo& confirmation, base::Time process_at));

  MOCK_METHOD(void,
              OnDidProcessConfirmationQueue,
              (const ConfirmationInfo& confirmation));

  MOCK_METHOD(void,
              OnFailedToProcessConfirmationQueue,
              (const ConfirmationInfo& confirmation));

  MOCK_METHOD(void, OnDidExhaustConfirmationQueue, ());
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_QUEUE_CONFIRMATION_QUEUE_DELEGATE_MOCK_H_

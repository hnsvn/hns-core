/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_CONFIRMATIONS_DELEGATE_MOCK_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_CONFIRMATIONS_DELEGATE_MOCK_H_

#include "hns/components/hns_ads/core/internal/account/confirmations/confirmations_delegate.h"

#include "testing/gmock/include/gmock/gmock.h"

namespace hns_ads {

struct ConfirmationInfo;

class ConfirmationsDelegateMock : public ConfirmationDelegate {
 public:
  ConfirmationsDelegateMock();

  ConfirmationsDelegateMock(const ConfirmationsDelegateMock&) = delete;
  ConfirmationsDelegateMock& operator=(const ConfirmationsDelegateMock&) =
      delete;

  ConfirmationsDelegateMock(ConfirmationsDelegateMock&&) noexcept = delete;
  ConfirmationsDelegateMock& operator=(ConfirmationsDelegateMock&&) noexcept =
      delete;

  ~ConfirmationsDelegateMock() override;

  MOCK_METHOD(void, OnDidConfirm, (const ConfirmationInfo& confirmation));

  MOCK_METHOD(void, OnFailedToConfirm, (const ConfirmationInfo& confirmation));
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_CONFIRMATIONS_DELEGATE_MOCK_H_

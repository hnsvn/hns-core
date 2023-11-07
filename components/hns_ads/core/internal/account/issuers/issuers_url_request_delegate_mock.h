/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_ISSUERS_ISSUERS_URL_REQUEST_DELEGATE_MOCK_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_ISSUERS_ISSUERS_URL_REQUEST_DELEGATE_MOCK_H_

#include "hns/components/hns_ads/core/internal/account/issuers/issuers_url_request_delegate.h"
#include "testing/gmock/include/gmock/gmock.h"

namespace hns_ads {

struct IssuersInfo;

class IssuersUrlRequestDelegateMock : public IssuersUrlRequestDelegate {
 public:
  IssuersUrlRequestDelegateMock();

  IssuersUrlRequestDelegateMock(const IssuersUrlRequestDelegateMock&) = delete;
  IssuersUrlRequestDelegateMock& operator=(
      const IssuersUrlRequestDelegateMock&) = delete;

  IssuersUrlRequestDelegateMock(IssuersUrlRequestDelegateMock&&) noexcept =
      delete;
  IssuersUrlRequestDelegateMock& operator=(
      IssuersUrlRequestDelegateMock&&) noexcept = delete;

  ~IssuersUrlRequestDelegateMock() override;

  MOCK_METHOD(void, OnWillFetchIssuers, (const base::Time fetch_at));

  MOCK_METHOD(void, OnDidFetchIssuers, (const IssuersInfo& issuers));

  MOCK_METHOD(void, OnFailedToFetchIssuers, ());

  MOCK_METHOD(void, OnWillRetryFetchingIssuers, (const base::Time retry_at));

  MOCK_METHOD(void, OnDidRetryFetchingIssuers, ());
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_ISSUERS_ISSUERS_URL_REQUEST_DELEGATE_MOCK_H_

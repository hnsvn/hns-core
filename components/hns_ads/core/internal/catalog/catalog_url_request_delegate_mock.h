/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CATALOG_URL_REQUEST_DELEGATE_MOCK_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CATALOG_URL_REQUEST_DELEGATE_MOCK_H_

#include "hns/components/hns_ads/core/internal/catalog/catalog_url_request_delegate.h"
#include "testing/gmock/include/gmock/gmock.h"

namespace hns_ads {

struct CatalogInfo;

class CatalogUrlRequestDelegateMock : public CatalogUrlRequestDelegate {
 public:
  CatalogUrlRequestDelegateMock();

  CatalogUrlRequestDelegateMock(const CatalogUrlRequestDelegateMock&) = delete;
  CatalogUrlRequestDelegateMock& operator=(
      const CatalogUrlRequestDelegateMock&) = delete;

  CatalogUrlRequestDelegateMock(CatalogUrlRequestDelegateMock&&) noexcept =
      delete;
  CatalogUrlRequestDelegateMock& operator=(
      CatalogUrlRequestDelegateMock&&) noexcept = delete;

  ~CatalogUrlRequestDelegateMock() override;

  MOCK_METHOD(void, OnWillFetchCatalog, (const base::Time fetch_at));

  MOCK_METHOD(void, OnDidFetchCatalog, (const CatalogInfo& catalog));

  MOCK_METHOD(void, OnFailedToFetchCatalog, ());

  MOCK_METHOD(void, OnWillRetryFetchingCatalog, (const base::Time retry_at));

  MOCK_METHOD(void, OnDidRetryFetchingCatalog, ());
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CATALOG_URL_REQUEST_DELEGATE_MOCK_H_

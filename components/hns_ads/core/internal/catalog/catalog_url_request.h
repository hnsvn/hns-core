/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CATALOG_URL_REQUEST_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CATALOG_URL_REQUEST_H_

#include "base/check_op.h"
#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "hns/components/hns_ads/core/internal/catalog/catalog_url_request_delegate.h"
#include "hns/components/hns_ads/core/internal/common/timer/backoff_timer.h"
#include "hns/components/hns_ads/core/internal/common/timer/timer.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"

namespace hns_ads {

struct CatalogInfo;

class CatalogUrlRequest final {
 public:
  CatalogUrlRequest();

  CatalogUrlRequest(const CatalogUrlRequest&) = delete;
  CatalogUrlRequest& operator=(const CatalogUrlRequest&) = delete;

  CatalogUrlRequest(CatalogUrlRequest&&) noexcept = delete;
  CatalogUrlRequest& operator=(CatalogUrlRequest&&) noexcept = delete;

  ~CatalogUrlRequest();

  void SetDelegate(CatalogUrlRequestDelegate* delegate) {
    CHECK_EQ(delegate_, nullptr);
    delegate_ = delegate;
  }

  void PeriodicallyFetch();

 private:
  void Fetch();
  void FetchCallback(const mojom::UrlResponseInfo& url_response);
  void FetchAfterDelay();

  void SuccessfullyFetchedCatalog(const CatalogInfo& catalog);
  void FailedToFetchCatalog();

  void Retry();
  void RetryCallback();
  void StopRetrying();

  raw_ptr<CatalogUrlRequestDelegate> delegate_ = nullptr;

  bool is_periodically_fetching_ = false;

  bool is_fetching_ = false;

  Timer timer_;
  BackoffTimer retry_timer_;

  base::WeakPtrFactory<CatalogUrlRequest> weak_factory_{this};
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CATALOG_URL_REQUEST_H_

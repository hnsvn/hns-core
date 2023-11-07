/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_GEOGRAPHICAL_SUBDIVISION_SUBDIVISION_URL_REQUEST_DELEGATE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_GEOGRAPHICAL_SUBDIVISION_SUBDIVISION_URL_REQUEST_DELEGATE_H_

#include <string>

#include "base/time/time.h"

namespace hns_ads {

class SubdivisionUrlRequestDelegate {
 public:
  // Invoked to tell the delegate we will fetch the subdivision at |fetch_at|.
  virtual void OnWillFetchSubdivision(const base::Time fetch_at) {}

  // Invoked to tell the delegate we successfully fetched the |subdivision|.
  virtual void OnDidFetchSubdivision(const std::string& subdvision) {}

  // Invoked to tell the delegate we failed to fetch the subdivision.
  virtual void OnFailedToFetchSubdivision() {}

  // Invoked to tell the delegate we will retry fetching the subdivision at
  // |retry_at|.
  virtual void OnWillRetryFetchingSubdivision(const base::Time retry_at) {}

  // Invoked to tell the delegate we retried fetching the subdivision.
  virtual void OnDidRetryFetchingSubdivision() {}

 protected:
  virtual ~SubdivisionUrlRequestDelegate() = default;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_GEOGRAPHICAL_SUBDIVISION_SUBDIVISION_URL_REQUEST_DELEGATE_H_

/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_ISSUERS_ISSUERS_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_ISSUERS_ISSUERS_INFO_H_

#include "hns/components/hns_ads/core/internal/account/issuers/issuer_info.h"

namespace hns_ads {

struct IssuersInfo final {
  IssuersInfo();

  IssuersInfo(const IssuersInfo&);
  IssuersInfo& operator=(const IssuersInfo&);

  IssuersInfo(IssuersInfo&&) noexcept;
  IssuersInfo& operator=(IssuersInfo&&) noexcept;

  ~IssuersInfo();

  bool operator==(const IssuersInfo&) const;
  bool operator!=(const IssuersInfo&) const;

  int ping = 0;
  IssuerList issuers;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_ISSUERS_ISSUERS_INFO_H_
